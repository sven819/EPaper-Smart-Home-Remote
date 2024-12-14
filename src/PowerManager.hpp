#include <functional>

class PowerManager
{
public:
    PowerManager(std::function<void()> shutdownCallback)
        : shutdownCallback(shutdownCallback)
    {
        BaseType_t createTask = xTaskCreatePinnedToCore([](void *pvParameters)
                                                        {
            PowerManager* powerManager = static_cast<PowerManager*>(pvParameters);
            powerManager->pwrMngmntTask(); },
                                                        "pwrMngmntTask", 4000, (void *)this, 10, NULL, 0);

        if (createTask != pdPASS)
        {
            uint32_t heap = esp_get_free_heap_size();
            ESP_LOGE("BOOT", "Task erstellen fehlgeschlagen, feier HEAP: %d", heap);
        }
    }
    static void initSleep()
    {
        ESP_LOGE("BOOT", "INIT SLEEP");
        TaskHandle_t taskHandle = xTaskGetHandle("pwrMngmntTask");
        if (taskHandle != nullptr)
            xTaskNotify(taskHandle, 2, eSetValueWithOverwrite);
    }

private:
    int sleepTimer = 60;
    std::function<void()> shutdownCallback;

    void pwrMngmntTask()
    {
        uint32_t ulNotificationValue = 0;

        for (;;)
        {
            TickType_t xMaxBlockTime = pdMS_TO_TICKS(this->sleepTimer * 1000);
            xTaskNotifyWait(0x00, ULONG_MAX, &ulNotificationValue, xMaxBlockTime);
            if (ulNotificationValue == 0)
            {
                if (shutdownCallback)
                {
                    shutdownCallback();
                }
            }
            else
            {
                ESP_LOGI("PowerManager", "Notification VAL : %u", ulNotificationValue);
                this->sleepTimer = 30;
                ulNotificationValue = 0;
            }
        }
    }
};
