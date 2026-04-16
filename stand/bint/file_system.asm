struc ext2_superblock
    .s_inodes_count       resd 1    ; total number of inodes
    .s_blocks_count       resd 1    ; total number of blocks
    .s_r_blocks_count     resd 1    ; reserved blocks
    .s_free_blocks_count  resd 1    ; free blocks
    .s_free_inodes_count  resd 1    ; free inodes
    .s_first_data_block   resd 1    ; first data block (usually 0 or 1)
    .s_log_block_size     resd 1    ; block size = 1024 << value
    .s_blocks_per_group   resd 1    ; blocks per group
    .s_inodes_per_group   resd 1    ; inodes per group
    .s_magic              resw 1    ; must be 0xEF53
endstruc

struc ext2_inode
    .i_mode      resw 1             ; file type + permissions
    .i_uid       resw 1             ; owner id
    .i_size      resd 1             ; file size in bytes
    .i_block     resd 12            ; direct block pointers
endstruc

struc ext2_group_desc
    .bg_inode_table resd 1          ; starting block of inode table
endstruc

struc ext2_dir_entry
    .inode     resd 1               ; inode number
    .rec_len   resw 1               ; entry size in bytes
    .name_len  resb 1               ; length of filename
    .name      resb 255             ; filename
endstruc



init_file_system:
    ; Read superblock:
    ; - Check magic number
    ; - Check block size
    ; Calculate inode table:
    ; - block_group_descriptor_table = superblock + block size
    ; - bg_inode_table -> base inode
    ; Get root inode:
    ; - inode2 = '/' (standard)
    ; Parse dir entries (hardcore):
    ; - inode | rec_len | name_len | name
    ; - /boot/kernel
    ;
    ; Pseudo code:
    ; init_file_system:
    ;   validate magic
    ;
    ;   load group descriptor
    ;   get inode table
    ;
    ;   inode = root_inode (2)
    ;
    ;   inode = find_child(inode, "boot")
    ;   inode = find_child(inode, "kernel")
    ;
    ;   load_inode_blocks_to_memory(inode, KERNEL_ADDR)
    ;
    ;   jmp KERNEL_ADDR
