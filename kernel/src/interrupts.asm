bits 32
HARDWARE_INT_OFFSET equ 0x20

.text:
%macro interruptMacro 1
    global interrupts_int%1
    interrupts_int%1:
        mov dword [interruptNumber], %1
        jmp interrupts.intHandler
%endmacro

interruptMacro 0
interruptMacro 1
interruptMacro 2
interruptMacro 3
interruptMacro 4
interruptMacro 5
interruptMacro 6
interruptMacro 7
interruptMacro 8
interruptMacro 9
interruptMacro 10
interruptMacro 11
interruptMacro 12
interruptMacro 13
interruptMacro 14
interruptMacro 15
interruptMacro 16
interruptMacro 17
interruptMacro 18
interruptMacro 19
interruptMacro 20
interruptMacro 21
interruptMacro 22
interruptMacro 23
interruptMacro 24
interruptMacro 25
interruptMacro 26
interruptMacro 27
interruptMacro 28
interruptMacro 29
interruptMacro 30
interruptMacro 31
interruptMacro 32
interruptMacro 33
interruptMacro 34
interruptMacro 35
interruptMacro 36
interruptMacro 37
interruptMacro 38
interruptMacro 39
interruptMacro 40
interruptMacro 41
interruptMacro 42
interruptMacro 43
interruptMacro 44
interruptMacro 45
interruptMacro 46
interruptMacro 47
interruptMacro 48
interruptMacro 49
interruptMacro 50
interruptMacro 51
interruptMacro 52
interruptMacro 53
interruptMacro 54
interruptMacro 55
interruptMacro 56
interruptMacro 57
interruptMacro 58
interruptMacro 59
interruptMacro 60
interruptMacro 61
interruptMacro 62
interruptMacro 63
interruptMacro 64
interruptMacro 65
interruptMacro 66
interruptMacro 67
interruptMacro 68
interruptMacro 69
interruptMacro 70
interruptMacro 71
interruptMacro 72
interruptMacro 73
interruptMacro 74
interruptMacro 75
interruptMacro 76
interruptMacro 77
interruptMacro 78
interruptMacro 79
interruptMacro 80
interruptMacro 81
interruptMacro 82
interruptMacro 83
interruptMacro 84
interruptMacro 85
interruptMacro 86
interruptMacro 87
interruptMacro 88
interruptMacro 89
interruptMacro 90
interruptMacro 91
interruptMacro 92
interruptMacro 93
interruptMacro 94
interruptMacro 95
interruptMacro 96
interruptMacro 97
interruptMacro 98
interruptMacro 99
interruptMacro 100
interruptMacro 101
interruptMacro 102
interruptMacro 103
interruptMacro 104
interruptMacro 105
interruptMacro 106
interruptMacro 107
interruptMacro 108
interruptMacro 109
interruptMacro 110
interruptMacro 111
interruptMacro 112
interruptMacro 113
interruptMacro 114
interruptMacro 115
interruptMacro 116
interruptMacro 117
interruptMacro 118
interruptMacro 119
interruptMacro 120
interruptMacro 121
interruptMacro 122
interruptMacro 123
interruptMacro 124
interruptMacro 125
interruptMacro 126
interruptMacro 127
interruptMacro 128
interruptMacro 129
interruptMacro 130
interruptMacro 131
interruptMacro 132
interruptMacro 133
interruptMacro 134
interruptMacro 135
interruptMacro 136
interruptMacro 137
interruptMacro 138
interruptMacro 139
interruptMacro 140
interruptMacro 141
interruptMacro 142
interruptMacro 143
interruptMacro 144
interruptMacro 145
interruptMacro 146
interruptMacro 147
interruptMacro 148
interruptMacro 149
interruptMacro 150
interruptMacro 151
interruptMacro 152
interruptMacro 153
interruptMacro 154
interruptMacro 155
interruptMacro 156
interruptMacro 157
interruptMacro 158
interruptMacro 159
interruptMacro 160
interruptMacro 161
interruptMacro 162
interruptMacro 163
interruptMacro 164
interruptMacro 165
interruptMacro 166
interruptMacro 167
interruptMacro 168
interruptMacro 169
interruptMacro 170
interruptMacro 171
interruptMacro 172
interruptMacro 173
interruptMacro 174
interruptMacro 175
interruptMacro 176
interruptMacro 177
interruptMacro 178
interruptMacro 179
interruptMacro 180
interruptMacro 181
interruptMacro 182
interruptMacro 183
interruptMacro 184
interruptMacro 185
interruptMacro 186
interruptMacro 187
interruptMacro 188
interruptMacro 189
interruptMacro 190
interruptMacro 191
interruptMacro 192
interruptMacro 193
interruptMacro 194
interruptMacro 195
interruptMacro 196
interruptMacro 197
interruptMacro 198
interruptMacro 199
interruptMacro 200
interruptMacro 201
interruptMacro 202
interruptMacro 203
interruptMacro 204
interruptMacro 205
interruptMacro 206
interruptMacro 207
interruptMacro 208
interruptMacro 209
interruptMacro 210
interruptMacro 211
interruptMacro 212
interruptMacro 213
interruptMacro 214
interruptMacro 215
interruptMacro 216
interruptMacro 217
interruptMacro 218
interruptMacro 219
interruptMacro 220
interruptMacro 221
interruptMacro 222
interruptMacro 223
interruptMacro 224
interruptMacro 225
interruptMacro 226
interruptMacro 227
interruptMacro 228
interruptMacro 229
interruptMacro 230
interruptMacro 231
interruptMacro 232
interruptMacro 233
interruptMacro 234
interruptMacro 235
interruptMacro 236
interruptMacro 237
interruptMacro 238
interruptMacro 239
interruptMacro 240
interruptMacro 241
interruptMacro 242
interruptMacro 243
interruptMacro 244
interruptMacro 245
interruptMacro 246
interruptMacro 247
interruptMacro 248
interruptMacro 249
interruptMacro 250
interruptMacro 251
interruptMacro 252
interruptMacro 253
interruptMacro 254
interruptMacro 255

interruptNumber:
    dd 0
esp_backup:
    dd 0
esp_backup2:
    dd 0
eax_backup:
    dd 0
extern handleInterrupt
global interrupts.intHandler
interrupts.intHandler:
    ;mov byte [0xb8000], 'A'
    ;mov byte [0xb8001], 11110000b
    ;jmp $
    cmp dword [interruptNumber], 8
    je continue
    cmp dword [interruptNumber], 10
    je continue
    cmp dword [interruptNumber], 11
    je continue
    cmp dword [interruptNumber], 12
    je continue
    cmp dword [interruptNumber], 13
    je continue
    cmp dword [interruptNumber], 14
    je continue
    cmp dword [interruptNumber], 17
    je continue
    cmp dword [interruptNumber], 30
    je continue

    push dword 0
    continue:

    mov [esp_backup2], esp

    push esp
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax

    push dword [interruptNumber]
    call handleInterrupt
    add esp, 4
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp
    pop dword [esp_backup]
    add esp, 4
    push eax
    mov eax, [esp_backup]
    cmp eax, [esp_backup2]
    pop eax
    
    je return
    mov [eax_backup],eax
    sub dword [esp_backup],12
    mov eax, [esp_backup]
    pop dword [eax]
    add dword [esp_backup],4
    mov eax, [esp_backup]
    pop dword [eax]
    add dword [esp_backup],4
    mov eax, [esp_backup]
    pop dword [eax]
    sub dword [esp_backup], 8

    mov eax, [eax_backup]
    mov esp, [esp_backup]
return:
    ;mov eax, [esp_backup]
    ;jmp $
    iret