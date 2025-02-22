# Cấu trúc thư muc tông quan
MicronOS/
│── base/             # Ứng dụng hệ thống và dịch vụ nền
│── boot/             # Bootloader và các thành phần liên quan đến quá trình khởi động
│── doc/              # Tài liệu hướng dẫn toàn bộ mã nguồn
│── drivers/          # Trình điều khiển thiết bị (Drivers)
│── kernel/           # Omicron Kernel của hệ điều hành ReactOS (giống Windows NT)
│── libs/             # Thư viện liên kết động (DLLs)
│── modules/          # Các mô-đun mở rộng hệ điều hành
│── sdk/              # Software Development Kit (SDK) chứa các API và công cụ phát triển
│── test/             # Test hệ thống
│── CMakeLists.txt    # Cấu hình CMake để biên dịch hệ điều hành
│── configure.cmd     # Cấu hình Build hệ điều hành
│── README.md         # Tài liệu hướng dẫn



Noritake Itron 256x128 VFD module
Shell UI

+-----------------------------+ boot.bin
|          Bootloader         |
+-----------------------------+ system.img
|   Micron Operating System   |
+-----------------------------+

Operating System - Folder Struck
│── Data/               # Dùng để lưu dữ liệu ứng dụng quan trọng, cần duy trì giữa các lần khởi động.
    │── App Data/       # Lưu trữ ảnh chụp từ camera hoặc ảnh tải về.
    │── Pictures/       # Lưu trữ ảnh chụp từ camera hoặc ảnh tải về.
    │── Music/          # Thư mục chứa nhạc và nội dung âm thanh.
    │── Videos/         # Chứa video quay hoặc tải về.
    │── Documents/      # Nơi lưu trữ các tài liệu văn phòng (Word, Excel, v.v.).
│── Program Data/       # Mỗi ứng dụng có một thư mục lưu trữ dữ liệu riêng trong Isolated Storage.
│── Program Files/      # Chứa các ứng dụng cài đặt trên hệ thống. Mỗi ứng dụng được lưu trữ trong một thư mục riêng biệt
    │── Micron Apps     # Ứng dụng được tải từ store
│── Recovery/           # Thư mục chứa chương trình recovery
│── System/             # Chua he dieu hanh
    │── Drivers/        # 
    │── Libs/           # 
    │── Bin/            # 
    │── Configs/        # 
    │── omicron.exec    # 
    │── ShellUI.exec    # 
    │── app.micpkg      #
│── Temp/               # Một số dữ liệu tạm thời hoặc cache của ứng dụng có thể được lưu ở đây.


    


    