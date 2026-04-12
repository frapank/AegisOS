; EXT2 Superblock Structure
struc ext2_superblock
    .s_inodes_count       resd 1       ; Total number of inodes (4 bytes)
    .s_blocks_count       resd 1       ; Total number of blocks (4 bytes)
    .s_r_blocks_count     resd 1       ; Reserved blocks count (4 bytes)
    .s_free_blocks_count  resd 1       ; Free blocks count (4 bytes)
    .s_free_inodes_count  resd 1       ; Free inodes count (4 bytes)
    .s_first_data_block   resd 1       ; First data block (4 bytes)
    .s_log_block_size     resd 1       ; Logarithmic block size (4 bytes)
    .s_log_frag_size      resd 1       ; Logarithmic fragment size (4 bytes)
    .s_blocks_per_group   resd 1       ; Blocks per group (4 bytes)
    .s_frags_per_group    resd 1       ; Fragments per group (4 bytes)
    .s_inodes_per_group   resd 1       ; Inodes per group (4 bytes)
    .s_mtime              resd 1       ; Time of last mount (4 bytes)
    .s_wtime              resd 1       ; Time of last write (4 bytes)
    .s_mnt_count          resw 1       ; Mount count (2 bytes)
    .s_max_mnt_count      resw 1       ; Max mount count before check (2 bytes)
    .s_magic              resw 1       ; EXT2 magic number (0xEF53, 2 bytes)
    .s_state              resw 1       ; Filesystem state (2 bytes)
    .s_errors             resw 1       ; Filesystem error handling (2 bytes)
    .s_minor_rev_level    resw 1       ; Minor revision level (2 bytes)
    .s_lastcheck          resd 1       ; Last check time (4 bytes)
    .s_checkinterval      resd 1       ; Max interval between checks (4 bytes)
    .s_creator_os         resd 1       ; Creator OS (Linux = 0x00010000, 4 bytes)
    .s_rev_level          resd 1       ; Filesystem revision level (4 bytes)
    .s_def_resuid         resw 1       ; Default reserved UID (2 bytes)
    .s_def_resgid         resw 1       ; Default reserved GID (2 bytes)
endstruc

; EXT2 Inode Structure
struc ext2_inode
    .i_mode           resw 1       ; File mode (permissions + type, 2 bytes)
    .i_uid            resw 1       ; User ID of owner (2 bytes)
    .i_size           resd 1       ; File size in bytes (4 bytes)
    .i_atime          resd 1       ; Last access time (4 bytes)
    .i_ctime          resd 1       ; Inode change time (4 bytes)
    .i_mtime          resd 1       ; Last modification time (4 bytes)
    .i_dtime          resd 1       ; Deletion time (if deleted, 4 bytes)
    .i_gid            resw 1       ; Group ID of owner (2 bytes)
    .i_links_count    resw 1       ; Hard link count (2 bytes)
    .i_blocks         resd 1       ; Number of allocated blocks (in 512-byte units, 4 bytes)
    .i_flags          resd 1       ; Inode flags (4 bytes)
    .i_reserved1      resd 1       ; Reserved for future use (4 bytes)
    .i_block          resd 15      ; Block pointers (15 entries, 4 bytes each)
endstruc

; EXT2 Group Descriptor
struc ext2_group_desc
    .bg_block_bitmap    resd 1       ; Indirizzo del bitmap dei blocchi (4 byte)
    .bg_inode_bitmap    resd 1       ; Indirizzo del bitmap degli inode (4 byte)
    .bg_inode_table     resd 1       ; Indirizzo della tabella degli inode (4 byte)
    .bg_free_blocks_count resw 1     ; Numero di blocchi liberi nel gruppo (2 byte)
    .bg_free_inodes_count resw 1     ; Numero di inodes liberi nel gruppo (2 byte)
    .bg_used_dirs_count resw 1       ; Numero di directory usate nel gruppo (2 byte)
    .bg_pad             resb 12      ; Padding per allineamento (12 byte)
endstruc

; EXT2 Directory Entry
struc ext2_dir_entry
    .inode              resd 1   ; Numero dell'inode (4 byte)
    .rec_len            resw 1   ; Lunghezza della voce di directory (2 byte)
    .name_len           resb 1   ; Lunghezza del nome del file (1 byte)
    .file_type          resb 1   ; Tipo di file (1 byte)
    .name               resb 255 ; Nome del file (255 byte)
endstruc

init_file_system:
    
