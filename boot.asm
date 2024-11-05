[bits 16]
[org 0x7c00]

jmp code
s: db "hello world!", 0  ; Ajout du caractère nul pour indiquer la fin de la chaîne

code:
mov bx, s

loop_code:
	mov ah, 0x0e           ; Fonction pour afficher un caractère dans le BIOS
	mov al, byte [bx]      ; Charger le caractère pointé par BX dans AL
	int 0x10               ; Appeler l'interruption BIOS pour afficher le caractère
	cmp byte [bx], 0       ; Comparer le caractère actuel avec 0 (fin de la chaîne)
	je end_loop            ; Si 0, on saute à la fin de la boucle
	inc bx                 ; Incrémenter BX pour avancer au caractère suivant
	jmp loop_code          ; Boucler

end_loop:
jmp $

times 510-($-$$) db 0
db 0x55
db 0xaa
