# Phần 2 · Cài đặt và kiểm định

**Người trình bày · Đỗ Tiến Thành** | **Thời lượng · 4 phút**

## 1. Tổ chức chương trình C · 1 phút

[Mã nguồn `trivium.c`](../trivium.c) dùng duy nhất hàm `main()`. Chương trình đọc đối số từ `argv`, tự đổi từng cặp ký tự hex thành một byte và kiểm tra độ dài. Phần xuất kết quả dùng `printf`, `fprintf` và `putchar` từ `stdio.h`.

Mảng `s[289]` bỏ trống ô 0 để chỉ số trong C trùng công thức. Vòng lặp chạy 1.152 nhịp đầu chỉ cập nhật trạng thái. Với các nhịp sau, nó XOR bit dòng khóa vào đúng vị trí bit trong mảng dữ liệu. Phép dịch toàn mảng từ cuối về đầu ghi đè các vị trí `s[94]` và `s[178]` bằng bit hồi tiếp ngay sau đó, nên tương đương với dịch riêng 3 thanh ghi.

## 2. Kiểm định bằng vector eSTREAM · 1 phút 30 giây

Bảng dùng **16 byte đầu** của mỗi vector. Bản rõ chung dài 16 byte toàn 0 (`00000000000000000000000000000000`), vì vậy đầu ra chính là 16 byte dòng khóa đầu tiên.

| Vector | Khóa hex | IV hex | Đầu ra mong đợi | Đầu ra thực tế | Kết quả |
|---|---|---|---|---|---|
| Set 1 #0 | `80000000000000000000` | `00000000000000000000` | `38EB86FF730D7A9CAF8DF13A4420540D` | `38EB86FF730D7A9CAF8DF13A4420540D` | PASS |
| Set 2 #0 | `00000000000000000000` | `00000000000000000000` | `FBE0BF265859051B517A2E4E239FC97F` | `FBE0BF265859051B517A2E4E239FC97F` | PASS |
| Set 6 #0 | `0053A6F94C9FF24598EB` | `0D74DB42A91077DE45AC` | `F4CD954A717F26A7D6930830C4E7CF08` | `F4CD954A717F26A7D6930830C4E7CF08` | PASS |

Chạy `./trivium` hoặc `.\trivium.exe` và chỉ ra hai dòng `EXPECT` và `OUTPUT`. Set 2 dùng khóa và IV toàn 0 làm mốc. Set 1 đổi một bit khóa. Set 6 có cả khóa lẫn IV khác 0 để kiểm tra thứ tự nạp bit của cả hai đầu vào. [Nguồn giá trị tham chiếu](https://algorithm.tamnd.com/trivium/#implementations) · [Log chạy thực tế](../tests/ket-qua.txt)

## 3. Demo mã hóa và giải mã · 1 phút

Dùng khóa và IV của Set 6 rồi chạy hai lệnh trong [README](../README.md#biên-dịch-và-chạy).

| Bước | Dữ liệu vào | Dữ liệu ra |
|---|---|---|
| Mã hóa | `5452495649554D` (ASCII `TRIVIUM`) | `A09FDC1C382A6B` |
| Giải mã | `A09FDC1C382A6B` | `5452495649554D` (ASCII `TRIVIUM`) |

Có thể kiểm tra bằng tay byte đầu tiên. Bản rõ `54` XOR dòng khóa `F4` bằng `A0`. Khi giải mã, `A0 XOR F4 = 54`.

## 4. Kết luận · 30 giây

Chương trình khớp 3/3 vector được chọn. Demo kiểm tra việc áp dụng phép XOR cho bản rõ khác 0. Nếu cả hai chiều cùng dùng sai một dòng khóa, demo vẫn có thể khôi phục đúng bản rõ, nên phép đối chiếu với vector tham chiếu là bước xác nhận đầu ra của thuật toán.
