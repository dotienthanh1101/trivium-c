# Bài tập lớn Lý thuyết mật mã · Nhóm 10

**Đề tài:** Tìm hiểu, cài đặt và kiểm định thuật toán mã dòng **Trivium** bằng C thuần.

## 1. Thành viên và phân công

| STT | Họ và tên | MSSV | Ngày sinh |
|:---:|---|:---:|:---:|
| 1 | **Đỗ Tiến Thành** | 20224150 | 11/01/2004 |
| 2 | **Lê Đức Hoàng** | 20233410 | 20/09/2005 |

| Thứ tự | Thành viên | Nội dung phụ trách | Minh họa |
|:---:|---|---|---|
| 1 | **Lê Đức Hoàng** | [Đặc tả Trivium](docs/01-thuat-toan.md), nạp khóa và IV, khởi tạo, cập nhật thanh ghi và sinh dòng khóa trong [mã C](trivium.c) | Tính nhịp đầu tiên với khóa và IV toàn 0 |
| 2 | **Đỗ Tiến Thành** | [Cài đặt và kiểm định](docs/02-cai-dat-va-kiem-dinh.md), xử lý dữ liệu hex, mã hóa và giải mã, đối chiếu vector eSTREAM và ghi [log thực nghiệm](tests/ket-qua.txt) | Chạy các vector và mã hóa, giải mã chuỗi `TRIVIUM` |

## 2. Tóm tắt thuật toán

Trivium là mã dòng đồng bộ do Christophe De Cannière và Bart Preneel thiết kế cho nhóm phần cứng của eSTREAM. Thuật toán dùng **khóa 80 bit**, **IV 80 bit** và **trạng thái 288 bit** chia thành ba thanh ghi dịch dài 93, 84 và 111 bit. Sau khi nạp khóa và IV, chương trình cập nhật trạng thái **1.152 nhịp** để khởi tạo. Từ nhịp tiếp theo, mỗi lần cập nhật sinh một bit dòng khóa.

Với bản rõ `P`, bản mã `C` và dòng khóa `Z`, phép mã hóa là `C = P XOR Z`. Giải mã dùng cùng khóa và IV để sinh lại `Z`, rồi tính `P = C XOR Z`. Xem sơ đồ thanh ghi, phương trình hồi tiếp và ví dụ tính một nhịp trong [Phần 1 · Thuật toán](docs/01-thuat-toan.md).

## 3. Cấu trúc và yêu cầu chương trình

| Tệp | Nội dung |
|---|---|
| [trivium.c](trivium.c) | Chương trình C11, đọc dữ liệu hex, sinh dòng khóa, XOR dữ liệu và chạy vector |
| [docs/01-thuat-toan.md](docs/01-thuat-toan.md) | Đặc tả, sơ đồ thanh ghi và ví dụ tính nhịp đầu |
| [docs/02-cai-dat-va-kiem-dinh.md](docs/02-cai-dat-va-kiem-dinh.md) | Ánh xạ thuật toán sang C, bảng Input/Output đầy đủ và kịch bản demo |
| [tests/ket-qua.txt](tests/ket-qua.txt) | Log chạy thực tế gồm khóa, IV, dữ liệu vào, đầu ra mong đợi, đầu ra thực tế và kết quả |

Toàn bộ chương trình nằm trong `main()`. Các lời gọi hàm thư viện chỉ gồm `printf`, `fprintf` và `putchar` từ `stdio.h`. Chương trình nhận đầu vào qua `argv` và thực hiện phép toán mật mã bằng mảng, vòng lặp và toán tử bit của C.

## 4. Biên dịch và chạy

Mở terminal tại thư mục gốc của repo. Chạy chương trình **không có đối số** để tự kiểm tra ba vector.

**Windows PowerShell với GCC**

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 trivium.c -o trivium.exe
.\trivium.exe
```

**Linux hoặc macOS**

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 trivium.c -o trivium
./trivium
```

Để mã hóa hoặc giải mã, truyền ba đối số theo thứ tự **khóa, IV, dữ liệu**. Khóa và IV đều gồm đúng 20 ký tự hex. Dữ liệu dài 1–256 byte, mỗi byte gồm hai ký tự hex. Chương trình chấp nhận cả chữ hoa và chữ thường.

```powershell
# Mã hóa ASCII TRIVIUM
.\trivium.exe 0053A6F94C9FF24598EB 0D74DB42A91077DE45AC 5452495649554D
# A09FDC1C382A6B

# Giải mã
.\trivium.exe 0053A6F94C9FF24598EB 0D74DB42A91077DE45AC A09FDC1C382A6B
# 5452495649554D
```

Trên Linux hoặc macOS, thay `.\trivium.exe` bằng `./trivium`. Mã thoát `0` là chạy thành công, `1` là có vector không khớp và `2` là đầu vào sai định dạng.

## 5. Kết quả kiểm định

Dùng bản rõ 16 byte toàn 0 để đầu ra chính là 16 byte đầu của dòng khóa. Mỗi dòng dưới đây đối chiếu kết quả chương trình với giá trị tham chiếu eSTREAM tương ứng.

| Vector | Giá trị mong đợi (hex) | Kết quả chương trình (hex) | Đánh giá |
|---|---|---|:---:|
| Set 1 #0 | `38EB86FF730D7A9CAF8DF13A4420540D` | `38EB86FF730D7A9CAF8DF13A4420540D` | PASS |
| Set 2 #0 | `FBE0BF265859051B517A2E4E239FC97F` | `FBE0BF265859051B517A2E4E239FC97F` | PASS |
| Set 6 #0 | `F4CD954A717F26A7D6930830C4E7CF08` | `F4CD954A717F26A7D6930830C4E7CF08` | PASS |

**Kết quả:** 3/3 vector khớp. Demo với bản rõ khác 0 giải mã về đúng chuỗi ban đầu. Xem [đầy đủ đầu vào và log chạy](tests/ket-qua.txt).

## 6. Tài liệu tham khảo

- Christophe De Cannière và Bart Preneel, *Trivium Specifications*, §2. [Đặc tả thuật toán](https://cr.yp.to/streamciphers/trivium/desc.pdf)
- ECRYPT Stream Cipher Project, Trivium 80-bit Key / 80-bit IV. [Bản lưu bộ test vector](https://github.com/bmkessler/trivium/blob/master/trivium-80.80.test-vectors)
- Quy ước thứ tự bit của khóa, IV và dòng khóa. [Mô tả và ví dụ đối chiếu](https://algorithm.tamnd.com/trivium/)
