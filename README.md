# Assignment10_Embedded_System
Đoạn mã FreeRTOS: Tác vụ Producer gửi dữ liệu qua Queue
📝 Tổng quan
Đoạn mã này minh họa một tác vụ (task) trong hệ điều hành thời gian thực FreeRTOS. Tác vụ này hoạt động như một "producer" (nhà sản xuất), có nhiệm vụ tạo ra dữ liệu một cách định kỳ và gửi nó đến một tác vụ khác thông qua một hàng đợi (queue).

Cứ mỗi 2 giây, tác vụ sẽ:

Chọn một giá trị tần số (frequency) từ một danh sách cho trước.

Tạo một giá trị chu kỳ nhiệm vụ (duty cycle) ngẫu nhiên.

Gửi cả hai giá trị này vào một hàng đợi có tên là xBlinkQueue.

In thông tin tần số và chu kỳ nhiệm vụ ra cổng USART1 để theo dõi.

⚙️ Chức năng chi tiết
Vòng lặp while(1) trong tác vụ thực hiện các công việc sau:

Lấy Tần số:

C

params.freq_hz = freq_list[i];
Gán giá trị tần số từ mảng freq_list tại chỉ số i vào biến params.freq_hz.

Tạo Duty Cycle Ngẫu nhiên:

C

params.duty_pct = (rand() % 81) + 10;
Tạo một số nguyên ngẫu nhiên trong khoảng từ 10 đến 90.

rand() % 81 sẽ cho kết quả từ 0 đến 80.

Cộng thêm 10 sẽ cho ra dải giá trị cuối cùng là 10 đến 90.

Gửi dữ liệu vào Queue:

C

xQueueSend(xBlinkQueue, &params, 0);
Sử dụng hàm API của FreeRTOS để gửi con trỏ của cấu trúc params vào hàng đợi xBlinkQueue. Tác vụ "consumer" (người tiêu dùng) khác sẽ nhận dữ liệu từ hàng đợi này để xử lý.

Gửi thông tin gỡ lỗi (Debug):

C

sprintf(msg, "Tanso: %d Hz | Duty: %d%%\r\n", params.freq_hz, params.duty_pct);
USART1_SendString(msg);
Định dạng một chuỗi chứa thông tin về tần số và duty cycle vừa tạo, sau đó gửi chuỗi này qua USART1 để người dùng có thể theo dõi trên terminal.

Cập nhật chỉ số mảng:

C

i = (i + 1) % (sizeof(freq_list)/sizeof(freq_list[0]));
Đây là một kỹ thuật phổ biến để duyệt vòng qua một mảng.

sizeof(freq_list)/sizeof(freq_list[0]) tính toán số lượng phần tử trong mảng freq_list.

Phép toán % (modulo) đảm bảo rằng khi i đạt đến phần tử cuối cùng, nó sẽ quay trở lại 0.

Tạm dừng tác vụ:

C

vTaskDelay(pdMS_TO_TICKS(2000));
Tạm dừng tác vụ trong 2000 mili giây (2 giây). Hàm vTaskDelay sẽ không chiếm dụng CPU, cho phép các tác vụ khác có độ ưu tiên thấp hơn được thực thi.
