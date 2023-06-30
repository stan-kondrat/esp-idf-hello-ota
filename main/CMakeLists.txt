idf_component_register(
    SRCS "main.c" "led.c" "wifi.c" "web.c"
    INCLUDE_DIRS "."
    EMBED_FILES "index.html"
    EMBED_TXTFILES "root_ca.pem"
)
