bits 32
org 0x00615690

inject:
    mov eax, [0x007150f4]
    push dllName
    call eax
    jmp 0x006158cf

dllName db 'Core.dll', 0