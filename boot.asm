[bits 16]
[org 0x7c00]

start:
    ; Définir les paramètres pour lire le deuxième secteur
    mov ah, 0x02      ; Fonction de lecture (0x02) de l'INT 0x13
    mov al, 1         ; Lire 1 secteur
    mov ch, 0         ; Piste (cylindre) 0
    mov dh, 0         ; Tête 0
    mov cl, 2         ; Secteur 2 (les secteurs commencent à 1, donc 2 est le suivant après le bootsector)

    ; Définir l'adresse de chargement : 0x7C00 + 512 = 0x7E00
    mov bx, 0x7c00+512    ; Adresse de destination (0x7C00 + 512)

    int 0x13          ; Appel BIOS pour lire le secteur

jmp code

code:
mov bx, 0x7c00+512

loop_code:
	mov ah, 0x0e
	mov al, byte [bx]
	int 0x10
	cmp byte [bx], '$'
	je end_loop
	inc bx
	jmp loop_code

end_loop:
jmp $



times 510-($-$$) db 0
db 0x55
db 0xaa

disk_start:
incbin "disk"
disk_end:
