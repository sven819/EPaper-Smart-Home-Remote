#include <cstring>

#define MAX_STRING_LENGTH 64

class AppState
{
public:
    AppState()
    {
        wakeup_reason = esp_sleep_get_wakeup_cause();
    }

    void setUiState(const std::string &state)
    {
        strncpy(uiState, state.c_str(), MAX_STRING_LENGTH - 1);
        uiState[MAX_STRING_LENGTH - 1] = '\0';
    }

    const char *getUiState() const
    {
        return uiState;
    }
    void setChecksum(uint32_t val)
    {
        checksum = val;
    }

    uint32_t getChecksum() const
    {
        return checksum;
    }

    // todo in HAL!
    esp_sleep_wakeup_cause_t wakeup_reason;
    void print_wakeup_reason()
    {
        static const char *TAG = "ESP";

        switch (wakeup_reason)
        {
        case ESP_SLEEP_WAKEUP_EXT0:
            ESP_LOGI(TAG, "Wakeup caused by external signal using RTC_IO");
            break;
        case ESP_SLEEP_WAKEUP_EXT1:
            ESP_LOGI(TAG, "Wakeup caused by external signal using RTC_CNTL");
            break;
        case ESP_SLEEP_WAKEUP_TIMER:
            ESP_LOGI(TAG, "Wakeup caused by timer");
            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            ESP_LOGI(TAG, "Wakeup caused by touchpad");
            break;
        case ESP_SLEEP_WAKEUP_ULP:
            ESP_LOGI(TAG, "Wakeup caused by ULP program");
            break;
        default:
            ESP_LOGI(TAG, "Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
            break;
        }
    }

private:
    static RTC_DATA_ATTR char uiState[MAX_STRING_LENGTH];
    static RTC_DATA_ATTR uint32_t checksum;
};

RTC_DATA_ATTR char AppState::uiState[MAX_STRING_LENGTH] = "";
RTC_DATA_ATTR uint32_t AppState::checksum = 0;