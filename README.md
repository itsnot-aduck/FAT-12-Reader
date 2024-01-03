# FAT-12-Reader
C Program to Read FAT12 Floppy Disk 


## tương tác với FAT File
- Mở file (folder, program) -> cat name
- Mở folder
- Thêm
- Xóa 
- (Optional) Đổi tên file

## Các hàm cần triển
cat - đọc dữ liệu từ 1 file
- FindName(char *name) => Trả về cluster, -1 nếu không phải

cd - chuyển folder 
- ChangeDir(char *name) 

Rm (void) - xóa file hoặc folder 
/* Check xem có extension hay không (check bằng dấu .) */
- RemoveDỉr()
     - Remove từng file 1 
     - Filename có byte đầu là E5
- Removefile
    - Free cluster list của nó. 

====> GetDataClusArr, GetFatClusAddr => Set lại giá trị về 0 (available)
- Hàm sửa giá trị các FAT Entry
FAT12_Fat_write(Fat entry, value)
