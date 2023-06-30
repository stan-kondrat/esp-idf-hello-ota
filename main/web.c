#include "esp_log.h"
#include "esp_partition.h"
#include "esp_ota_ops.h"
#include "esp_tls.h"
#include "cJSON.h"
#include "web.h"
#include "ota.h"
#include "ota_github.h"

#include <sys/param.h>


static const char *TAG = "WEB";

httpd_handle_t web_server;
ota_github_config_t global_config = {0};

// HTTP GET ROOT Handler
extern const char index_start[] asm("_binary_index_html_start");
extern const char index_end[] asm("_binary_index_html_end");

static esp_err_t root_get_handler(httpd_req_t *req) {
    const uint32_t root_len = index_end - index_start;
    ESP_LOGI(TAG, "Serve root");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, index_start, root_len);
    return ESP_OK;
}
static const httpd_uri_t root_uri = {.uri = "/", .method = HTTP_GET, .handler = root_get_handler};

// HTTP GET API/STATE Handler

static void parse_query_config(char *buf, ota_github_config_t *ret_config) {
    ESP_LOGI(TAG, "Found URL query => %s", buf);
    char param[64] = {0};
    /* Get value of expected key from query string */
    if (httpd_query_key_value(buf, "github_user", param, sizeof(ret_config->github_user)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => github_user=%s", param);
        strncpy((char *)ret_config->github_user, param, sizeof(ret_config->github_user));
    }
    if (httpd_query_key_value(buf, "github_repo", param, sizeof(ret_config->github_repo)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => github_repo=%s", param);
        strncpy((char *)ret_config->github_repo, param, sizeof(ret_config->github_repo));
    }
    if (httpd_query_key_value(buf, "filename", param, sizeof(ret_config->filename)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => filename=%s", param);
        strncpy((char *)ret_config->filename, param, sizeof(ret_config->filename));
    }
    if (httpd_query_key_value(buf, "current_version", param, sizeof(ret_config->current_version)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => current_version=%s", param);
        strncpy((char *)ret_config->current_version, param, sizeof(ret_config->current_version));
    }
    if (httpd_query_key_value(buf, "newer", param, sizeof(param)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => newer=%s", param);
        ret_config->newer = strcmp(param, "true")==0;
    }
    if (httpd_query_key_value(buf, "latest", param, sizeof(param)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => latest=%s", param);
        ret_config->latest = strcmp(param, "true")==0;
    }
    if (httpd_query_key_value(buf, "prerelease", param, sizeof(param)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => prerelease=%s", param);
        ret_config->prerelease = strcmp(param, "true")==0;
    }
    if (httpd_query_key_value(buf, "release_id", param, sizeof(param)) == ESP_OK) {
        ESP_LOGI(TAG, "Found URL query parameter => release_id=%s", param);
        ret_config->release_id = atoll((char *)param);
        ESP_LOGI(TAG, "Found URL query parameter => release_id=%lld", ret_config->release_id);
    }
}

static esp_err_t api_get_handler(httpd_req_t *req) {

    httpd_resp_set_type(req, "application/json");
    cJSON *json = cJSON_CreateObject();

    // get current version
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    ESP_ERROR_CHECK(esp_ota_get_partition_description(running, &running_app_info));
    ESP_LOGI(TAG, "Running firmware version: %s", running_app_info.version);
    cJSON_AddStringToObject(json, "current_version", (char *)running_app_info.version);

    // get releases
    cJSON *releaseItems = cJSON_AddArrayToObject(json, "releases");

    /* Read URL query string length and allocate memory for length + 1,
     * extra byte for null termination */
    size_t buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        char*  buf;
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            parse_query_config(buf, &global_config);
        }
        free(buf);
    }

    ota_github_releases_t* releases = ota_github_get_releases(&global_config);
    for (int i = 0; i < releases->size; i++)
    {
        cJSON *releaseItem = cJSON_CreateObject();
        cJSON_AddNumberToObject(releaseItem, "id", (long)releases->releases[i].id);
        cJSON_AddStringToObject(releaseItem, "name", (char *)releases->releases[i].name);
        cJSON_AddStringToObject(releaseItem, "tag_name", (char *)releases->releases[i].tag_name);
        cJSON_AddStringToObject(releaseItem, "created_at", (char *)releases->releases[i].created_at);
        cJSON_AddItemToArray(releaseItems, releaseItem);
    }

    const char *response = cJSON_Print(json);
    httpd_resp_sendstr(req, response);
    free((void *)response);
    cJSON_Delete(json);
    return ESP_OK;
}
static const httpd_uri_t api_uri = {.uri = "/api", .method = HTTP_GET, .handler = api_get_handler};


// HTTP POST Update Handler
static esp_err_t update_post_handler(httpd_req_t *req) {
    char buf[256];

    ESP_LOGI(TAG, "update_post_handler req->content_len = %d", req->content_len);

    /* Read data received in the request */
    int ret = httpd_req_recv(req, buf, MIN(req->content_len, sizeof(buf)));
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }

    char help_please_wait[] = "<pre>Please wait...\n";
    httpd_resp_send_chunk(req, help_please_wait, sizeof(help_please_wait));

    ESP_LOGI(TAG, "POST URL body: %s \n", buf);
    parse_query_config(buf, &global_config);

    ota_github_releases_t* releases = ota_github_get_releases(&global_config);
    ESP_LOGI(TAG, "releases->size=%d", releases->size);
    if (releases->size != 1) {
        char help_wrong_id[] = "\n Release id not found";
        httpd_resp_send_chunk(req, (char*)help_wrong_id, sizeof(help_wrong_id));
        ESP_LOGI(TAG, "%s %lld \n", help_wrong_id, global_config.release_id);
        httpd_resp_send_chunk(req, NULL, 0);
        return ESP_FAIL;
    }

    char help_download[] = "\nDownload url: ";
    httpd_resp_send_chunk(req, (char*)help_download, sizeof(help_download));
    httpd_resp_send_chunk(req, (char*)releases->releases[0].download_url, sizeof(releases->releases[0].download_url));
    ESP_LOGI(TAG, "%s %s \n", help_download, releases->releases[0].download_url);

    ota_config_t ota_config = {0};
    strncpy((char *)ota_config.url, (char *)releases->releases[0].download_url, 256);
    ota_install(ota_config);

    char help_updating[] = "<pre>Updating...\n";
    httpd_resp_send_chunk(req, help_updating, sizeof(help_updating));

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}
static const httpd_uri_t udpate_uri = {.uri = "/update", .method = HTTP_POST, .handler = update_post_handler};

void web_server_start(void) {
    httpd_handle_t server = NULL;
    httpd_config_t httpd_config = HTTPD_DEFAULT_CONFIG();
    httpd_config.stack_size = 8000;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", httpd_config.server_port);
    if (httpd_start(&server, &httpd_config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &root_uri);
        httpd_register_uri_handler(server, &api_uri);
        httpd_register_uri_handler(server, &udpate_uri);
    }
}
