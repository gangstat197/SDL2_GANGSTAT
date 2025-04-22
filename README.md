# MICRO DASH: Bài tập Lập trình Nâng cao INT2215
<p align="center">
  <img src="https://github.com/user-attachments/assets/23c56329-f7bd-4c7e-93d0-415a5bafad05" width="300" alt="MicroDash_Hero" />
</p>



## I. Thông tin tác giả 
- **Họ và tên:** Lê Đức Lưu 
- **Mã số sinh viên:** 24021557
- **Lớp:** K69I - CS1

## II. Tổng quan 
- **Video Demo Game:** [Micro Dash](https://youtu.be/4HtiXw1boTk). 
- **Thể loại:** Infinite Loops.
- **Nền tảng:** PC.
- **Ngôn ngữ / Thư viện sử dụng:** C++ / SDL2. 
- **Tóm tắt:** Trong _Micro Dash_, người chơi sẽ phải dùng chuột điều khiển nhân vật để tránh né các loại **quái vật không gian**. Các loại quái vật nguy hiểm khác nhau về hình dạng, kích thước, hành vi, tốc độ. Trong quá trình chơi, người chơi có thể nhận được các **Nâng cấp đặc biệt** như giảm kích thước để dễ dàng tránh né, tăng lượng điểm nhận được, hoặc là bất tử. Càng ngày sẽ có càng nhiều quái vật xuất hiện, và _số điểm nhận được sẽ tỉ lệ thuận với thời gian sinh tồn_ của người chơi.
- **Ngưỡng điểm:** 9 - 10.

## III. Các điểm nổi bật của Game.
### 1. Kiến trúc Game Engine
Dự án tập trung vào việc thiết kế một kiến trúc game engine đơn giản sử dụng SDL2, với cấu trúc rõ ràng và hướng đến tính mô-đun hoá cao. Hệ thống được tổ chức quanh các class manager tự động, giúp điều phối các thành phần chính của trò chơi một cách hiệu quả: 
<p align="center">
  <img src="https://github.com/user-attachments/assets/e0133249-75e0-4f13-800c-e3d9b2f8edab" width="200" />
</p>



Nhờ nền tảng kiến trúc được xây dựng bài bản ngay từ đầu, việc tích hợp và mở rộng các chức năng trong game trở nên linh hoạt, dễ dàng. Game loop được tổ chức chặt chẽ, đảm bảo quá trình xử lý diễn ra mượt mà và nhất quán.

### 2. Quái vật đa dạng & Thuật toán sinh ngẫu nhiên quái vật

#### Quái vật

<p align="center">
  <img src="https://github.com/user-attachments/assets/2da3ea48-c2e2-4fef-aa2c-8bcd0a77e65f" width="100"/>
  <img src="https://github.com/user-attachments/assets/21ce331c-dd1b-4548-a8be-40717e08b337" width="100"/>
  <img src="https://github.com/user-attachments/assets/e3b6dde3-3e32-4246-bb3f-f4e35c6231bc" width="100"/>
  <img src="https://github.com/user-attachments/assets/41feb91e-94b4-4e10-bbfd-7a399cc0b60b" width="100"/>
  <img src="https://github.com/user-attachments/assets/1e00dfbe-458d-44b1-ae09-a0c96b219487" width="100"/>
  <img src="https://github.com/user-attachments/assets/826c9eff-c278-4258-a694-76d32cefa3d9" width="100"/>
</p>

Có tổng cộng **6 loại quái vật** trong game, mỗi loại sở hữu:
- Hình dáng, kích thước, tốc độ, góc quay và hành vi di chuyển riêng biệt.  
- **3 kiểu chuyển động:** _Đi thẳng_, _Đi cong_, và _Đi zíc-zắc_ — giúp tăng tính thử thách trong từng màn chơi.

---

#### Thuật toán sinh ngẫu nhiên quái vật

Game sử dụng thuật toán sinh bản sao từ 6 loại quái vật gốc.

- **Càng về sau**, **tần suất sinh quái vật càng tăng** theo thời gian.  
- Điều này giúp tạo ra một nhịp độ chơi gấp gáp, đồng thời tăng dần độ khó cho người chơi trong suốt hành trình Micro Dash.


### 3. Nâng cấp đa dạng
Có toàn bộ **3 loại nâng cấp** cho người chơi, được sinh ngẫu nhiên trong trận đấu. Các loại nâng cấp: 

<table>
  <tr>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/e42cbd80-9900-497d-ba30-68c5e801fc70" width="100" /><br/>
      <em>AntMan</em><br/>
      Thu nhỏ kích thước của nhân vật để dễ dàng tránh né hơn.
    </td>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/7b7c30eb-4e67-4646-a224-8d43e2769ab3" width="100" /><br/>
      <em>Midas</em><br/>
      Số điểm nhận được mỗi giây tăng lên gấp 2 lần.
    </td>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/61e1ca49-3feb-4f43-8106-680e6dfcdeb6" width="100" /><br/>
      <em>Invincible</em><br/>
      Trở nên bất tử trong một khoảng thời gian.
    </td>
  </tr>
</table>

### 4. Collider cho hình đa diện lồi và Thuật toán kiểm tra va chạm Separate Axis Thereom (SAT)
Có thể tạo Collider cho các hình đa diện lồi phức tạp:
<p align="center">
  <img src="https://github.com/user-attachments/assets/a25de0ec-0a5f-4f2b-8267-129d7c9997db" width="150" height="150" />
  <img src="https://github.com/user-attachments/assets/c029ec1b-c500-411f-ad32-883337f1ba26" width="150" height="150" />
</p>

Xử lý tính toán cho các đỉnh xoay quanh điểm trung tâm, và di chuyển theo hành vi định sẵn.

---

**Thuật toán Separate Axis Thereom (SAT):**
<p align="center">
  <img src="https://github.com/user-attachments/assets/8b8676e2-7d38-4919-8fc5-194b7ee0d391" width="250" /><br />
  <em>Ảnh trong game: Va chạm giữa Nhân vật và Quái vật</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/355675b2-8aeb-40f0-8a15-9b2af6e69c88" width="300" /><br />
  <em>Thuật toán SAT: Mô tả về phương pháp kiểm tra va chạm giữa các đối tượng trong game</em>
</p>

### 5. Hệ thống Animation đa dạng 
- **Trail Animation** cho các vật thể trong Game: Để tạo cảm giác _mượt mà_ và _tốc độ_ cho Game. Bằng cách thêm vào dấu đường đi của nhân vật và các vật thể.
- **UI Animation:** [Demo](https://github.com/user-attachments/assets/2be4a335-4cc1-49b7-8081-0904bf480eb3)
- **Hiệu ứng cuộn vô hạn của Background:** Tạo cảm giác không gian rộng mở và chuyển động liền mạch trong suốt quá trình chơi.
### 6. Hệ thống Âm thanh 

_Demo âm thanh ở Video Demo phía trên._ 
- Âm nhạc nền 
- Âm thanh cho các nút: Hover, Click
- Âm thanh cho người chơi nhận nâng cấp
- Âm thanh bị quái vật va trúng
### 7. Hệ thống User Interface (UI)
<p align="center">
  <img src="https://github.com/user-attachments/assets/febba627-671c-4dae-a294-5a8c86fb3081" width="200" height="320" /><br/>
  <em>Transition Button – chuyển cảnh mượt mà giữa các màn chơi</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/43e2446f-b412-4843-8e28-aa229b105df7" width="200" height="150" /><br/>
  <em>Action Button – thao tác chức năng nhanh chóng và rõ ràng</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/51c8a1b3-0b22-48d2-b39e-249735f7fb4c" width="200" height="150" /><br/>
  <em>Screen Title – hiển thị tiêu đề nổi bật, định hướng người chơi</em>
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/858e7334-a290-4555-9433-09663b789d77" width="200" height="150" /><br/>
  <em>Mouse Pointer – giao diện chuột được tùy biến phù hợp với phong cách game</em>
</p>

### 8. Chức năng Lưu bảng điểm xếp hạng (Highscore). 
**Lưu trữ thông tin Highscore trong file nhị phân**: `highscore.dat`.  
_Sau khi hoàn thành lượt chơi_, kết quả sẽ được ghi vào file.

Người chơi có thể xem được **5 điểm** cao nhất, và _có thể reset lại_ để bắt đầu lại từ đầu.  

<p align="center">
  <img src="https://github.com/user-attachments/assets/5fee7c44-a1eb-4bd1-9e0c-8736f0407393" width="400" />
</p>

## IV. Tài liệu tham khảo
- Tham khảo Code mẫu trên [Lazy Foo' Productions](https://lazyfoo.net/tutorials/SDL/index.php).
- Tham khảo Code mẫu trên [File tài liệu do giảng viên cung cấp](https://lazyfoo.net/tutorials/SDL/index.php).
- Tham khảo Code mẫu trên [trang chính thức của SDL2](https://www.libsdl.org/).
- Một số hình ảnh được lấy từ [Freepik](https://www.freepik.com/). 
- Toàn bộ hiệu ứng âm thanh được từ [Pixabay](https://pixabay.com). 
Ngoài ra, không sử dụng code ở những nguồn không được liệt kê ở trên
