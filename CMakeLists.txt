# ESP-IDF component (add this repo as a component or via idf_component.yml).
idf_component_register(
    INCLUDE_DIRS "src"
    REQUIRES led_strip esp_timer
)
