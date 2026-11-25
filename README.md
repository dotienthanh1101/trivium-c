# Mã dòng Trivium bằng C thuần

**Nhóm thực hiện** · Lê Đức Hoàng và Đỗ Tiến Thành

Trivium là mã dòng đồng bộ thuộc nhóm phần cứng của eSTREAM. Chương trình nhận khóa, IV và dữ liệu ở dạng hex. Biên dịch theo C11, toàn bộ chương trình nằm trong `main()` và chỉ gọi các hàm nhập/xuất chuẩn từ `stdio.h`.

## Bố cục và phân công trình bày

| Thành viên | Thời lượng | Nội dung | Demo |
|---|---:|---|---|
| **Lê Đức Hoàng** | 4 phút | Nguyên lý mã dòng, cấu trúc trạng thái, nạp khóa và IV, tính đầu ra và hồi tiếp | Minh họa nhịp đầu tiên với khóa và IV toàn 0 |
| **Đỗ Tiến Thành** | 4 phút | Ánh xạ thuật toán sang C, chạy vector eSTREAM, phân tích kết quả | Chạy kiểm thử và mã hóa/giải mã chuỗi `TRIVIUM` |

1. [Phần 1 · Thuật toán — Lê Đức Hoàng](docs/01-thuat-toan.md)
2. [Phần 2 · Cài đặt và kiểm định — Đỗ Tiến Thành](docs/02-cai-dat-va-kiem-dinh.md)
3. [Mã nguồn C](trivium.c) · [Kết quả chạy](tests/ket-qua.txt)

## Biên dịch và chạy

Với GCC trên Windows PowerShell, chạy tại thư mục dự án.

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 trivium.c -o trivium.exe
.\trivium.exe
```

Trên Linux hoặc macOS, dùng `cc`.

```sh
cc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 trivium.c -o trivium
./trivium
```

Chạy chương trình không có đối số sẽ kiểm tra 3 vector. Để mã hóa hoặc giải mã, truyền 3 đối số theo thứ tự **khóa, IV, dữ liệu**. Khóa và IV đều có 20 ký tự hex. Dữ liệu dài từ 1 đến 256 byte, biểu diễn bằng 2 ký tự hex cho mỗi byte, giữ cả các số 0 ở đầu. Chương trình chấp nhận chữ hoa hoặc chữ thường.

```powershell
.\trivium.exe 0053A6F94C9FF24598EB 0D74DB42A91077DE45AC 5452495649554D
# A09FDC1C382A6B
.\trivium.exe 0053A6F94C9FF24598EB 0D74DB42A91077DE45AC A09FDC1C382A6B
# 5452495649554D
```

Trên Linux hoặc macOS, thay `./trivium` vào vị trí lệnh chạy. Mã thoát `0` nghĩa là chạy thành công, `1` nghĩa là có vector không khớp và `2` nghĩa là đầu vào sai định dạng.
