# Assignment10_Embedded_System
FreeRTOS PWM Random Generator with UART Logging
🧩 Giới thiệu

Chương trình này chạy trên vi điều khiển (MCU) sử dụng FreeRTOS, có nhiệm vụ:

Tự động thay đổi tần số (frequency) và chu kỳ nhiệm vụ (duty cycle) của tín hiệu PWM một cách ngẫu nhiên.

Gửi thông tin hiện tại qua UART1 để theo dõi hoặc debug trên Serial Monitor.

Luân phiên thay đổi giữa các giá trị trong mảng freq_list.

🔄 Nguyên lý hoạt động

Trong task chính, vòng lặp while(1) thực hiện quy trình sau:

char msg[50];

while(1)
{
    params.freq_hz = freq_list[i];               // Lấy tần số hiện tại
    params.duty_pct = (rand() % 81) + 10;        // Sinh duty ngẫu nhiên 10–90%

    xQueueSend(xBlinkQueue, &params, 0);         // Gửi dữ liệu qua Queue cho task PWM

    sprintf(msg, "Tanso: %d Hz | Duty: %d%%\r\n",
            params.freq_hz, params.duty_pct);
    USART1_SendString(msg);                      // In thông tin qua UART

    i = (i + 1) % (sizeof(freq_list) / sizeof(freq_list[0]));  // Tần số kế tiếp
    vTaskDelay(pdMS_TO_TICKS(2000));             // Delay 2 giây
}
Tạm dừng tác vụ trong 2000 mili giây (2 giây). Hàm vTaskDelay sẽ không chiếm dụng CPU, cho phép các tác vụ khác có độ ưu tiên thấp hơn được thực thi.
Yêu cầu phần mềm

FreeRTOS (sử dụng xQueueSend(), vTaskDelay()).

UART1 đã được cấu hình sẵn để truyền chuỗi.

PWM Control Task riêng biệt đang nhận dữ liệu từ xBlinkQueue.
