# Phần 1 · Thuật toán Trivium

**Người trình bày · Lê Đức Hoàng** | **Thời lượng · 4 phút**

## 1. Nguyên lý mã dòng · 45 giây

Trivium nhận khóa bí mật 80 bit và IV 80 bit để sinh dòng khóa. Với bản rõ `P`, dòng khóa `Z` và bản mã `C`, phép mã hóa là `C = P XOR Z`. Bên nhận khởi tạo lại bằng cùng khóa và IV rồi tính `P = C XOR Z` vì `Z XOR Z = 0`. Khi bản rõ toàn 0, bản mã bằng đúng dòng khóa. Tính chất này được dùng trong bộ kiểm thử.

## 2. Trạng thái và khởi tạo · 1 phút

Trạng thái 288 bit chia thành 3 thanh ghi dịch. Ký hiệu `s[i]` dùng chỉ số từ 1 theo đặc tả.

| Thanh ghi | Vị trí trong trạng thái | Giá trị ban đầu |
|---|---|---|
| A · 93 bit | `s[1]…s[93]` | 80 bit khóa rồi 13 bit 0 |
| B · 84 bit | `s[94]…s[177]` | 80 bit IV rồi 4 bit 0 |
| C · 111 bit | `s[178]…s[288]` | 108 bit 0 rồi 3 bit 1 |

Sau khi nạp, cập nhật trạng thái **1.152 nhịp** và bỏ đầu ra. Nhịp **1.153** sinh bit dòng khóa hợp lệ đầu tiên. Trong định dạng byte của vector eSTREAM, bit cao nhất của byte hex cuối của khóa vào `s[1]`, bit thấp nhất của byte đầu vào `s[80]`. IV dùng cùng quy tắc cho `s[94]…s[173]`. Bit dòng khóa đầu tiên vào bit thấp nhất của byte đầu ra đầu tiên.

## 3. Phép tính mỗi nhịp · 1 phút 30 giây

Từ trạng thái hiện tại, tính các tổ hợp đầu ra trước, sau đó tính hồi tiếp. Ký hiệu `^` là XOR và `&` là AND.

```text
t1 = s[66]  ^ s[93]
t2 = s[162] ^ s[177]
t3 = s[243] ^ s[288]
z  = t1 ^ t2 ^ t3

t1 = t1 ^ (s[91]  & s[92])  ^ s[171]
t2 = t2 ^ (s[175] & s[176]) ^ s[264]
t3 = t3 ^ (s[286] & s[287]) ^ s[69]
```

Dịch các bit bên trong mỗi thanh ghi về phía chỉ số lớn hơn, bỏ bit cuối cũ, rồi đưa `t3` vào đầu A, `t1` vào đầu B và `t2` vào đầu C. Phải tính xong cả ba hồi tiếp trước khi sửa trạng thái. Các phép AND tạo thành phần hồi tiếp phi tuyến.

## 4. Minh họa nhịp đầu · 45 giây

Với khóa và IV toàn 0, vị trí duy nhất có giá trị 1 lúc nạp là `s[286]`, `s[287]`, `s[288]`. Ở nhịp đầu, trước khi tính hồi tiếp, `t1 = 0`, `t2 = 0`, `t3 = 1`. Sau hồi tiếp, `t3 = 1 XOR (1 AND 1) XOR 0 = 0`. Bit `z = 1` của nhịp này bị bỏ vì vẫn đang khởi tạo. Chuyển sang phần demo để đối chiếu đầu ra sau khi đủ 1.152 nhịp.

**Tài liệu tham khảo** · [De Cannière và Preneel, Trivium Specifications](https://web.archive.org/web/20161020205326/http://www.ecrypt.eu.org/stream/ciphers/trivium/trivium.pdf) · [Mô tả thuật toán và quy ước thứ tự bit](https://algorithm.tamnd.com/trivium/)
