# Assignment10_Embedded_System
STM32 FreeRTOS PWM Simulation with UART Logging
🧩 Giới thiệu

Dự án này mô phỏng hoạt động của tín hiệu PWM thay đổi ngẫu nhiên trên vi điều khiển STM32F103C8T6, được điều khiển bởi FreeRTOS.
Chương trình bao gồm hai nhiệm vụ (task) chạy song song:

Task Generate: Tạo ngẫu nhiên tần số và duty cycle, gửi thông tin qua hàng đợi (queue).

Task Blink: Nhận thông tin từ queue và nháy LED tương ứng với tín hiệu PWM giả lập.

Mỗi giá trị PWM được gửi sẽ đồng thời được in ra UART1 để người dùng quan sát trên Serial Monitor.

⚙️ Cấu trúc hệ thống

Toàn bộ chương trình được xây dựng trên kiến trúc FreeRTOS với hai task chính và một hàng đợi trung gian:

vTaskGenerate():

Lấy tần số từ mảng freq_list[] = {1, 2, 4, 8, 10} (Hz).

Sinh ngẫu nhiên duty cycle trong khoảng 10% – 90%.

Gửi thông số {tần số, duty} đến queue xBlinkQueue.

In ra UART1 chuỗi thông báo như:

Tanso: 4 Hz | Duty: 65%


vTaskBlink():

Nhận thông số từ queue.

Tính chu kỳ sáng và tắt LED dựa trên duty cycle.

Nháy LED tại chân PC13 với thời gian sáng/tắt tương ứng.

LED sáng càng lâu thì duty càng cao, tạo hiệu ứng PWM trực quan.

🧠 Nguyên lý hoạt động

Khởi tạo hệ thống:
Hàm SystemInit() thiết lập xung nhịp, GPIO_Config() cấu hình chân PC13 làm ngõ ra,
USART1_Config() cấu hình UART1 (TX: PA9, RX: PA10, baudrate 9600 bps).

Tạo hàng đợi và task:
Hàng đợi xBlinkQueue được tạo để chứa 5 phần tử kiểu blink_param_t, gồm tần số và duty.
Hai task được khởi tạo với độ ưu tiên khác nhau:

Task Blink (ưu tiên cao hơn).

Task Generate (ưu tiên thấp hơn).

Vòng lặp vô hạn của mỗi task:

vTaskGenerate() gửi dữ liệu mới mỗi 2 giây.

vTaskBlink() luôn chờ nhận dữ liệu để thay đổi kiểu nháy LED tương ứng.

Bộ lập lịch FreeRTOS (Scheduler) sẽ tự động phân chia thời gian CPU cho hai task.

💡 Ví dụ hoạt động

Khi chạy chương trình, cổng UART1 sẽ gửi thông tin ra terminal (9600 bps) như sau:

Tanso: 1 Hz | Duty: 25%
Tanso: 2 Hz | Duty: 67%
Tanso: 4 Hz | Duty: 41%
Tanso: 8 Hz | Duty: 89%
Tanso: 10 Hz | Duty: 54%


LED trên bo mạch sẽ sáng – tắt theo tần số và duty tương ứng, tạo hiệu ứng chớp nháy ngẫu nhiên.

FreeRTOS (sử dụng xQueueSend(), vTaskDelay()).

UART1 đã được cấu hình sẵn để truyền chuỗi.

PWM Control Task riêng biệt đang nhận dữ liệu từ xBlinkQueue.
