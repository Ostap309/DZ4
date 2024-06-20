global f1
global f2
global f3
section .data

    minus:      dq	-1.0
    f1_const:   dq	3.0
    f2_const:   dq	2.0
    f3_const:   dq	1.0

section .text	
f1:
    fld qword[esp+4]        ; читаем x
    fmul qword[minus]       ; умножаем на -1
    fldl2e                  ; грузим 2*log_2(e)
    fmulp st1,st0           ; перемножаем
    fld1                    ; грузим 1
    fscale                  ; Возводим в степень двойки
    fxch                    ; Меняем
    fld1                    ; Грузим 1
    fxch                    ; Меняем
    fprem                   ; Остаток от деления
    f2xm1                   ; 2^st0-1
    faddp st1,st0           ; прибавляем единицу
    fmulp st1,st0           ; перемножаем
    fadd qword[f1_const]    ; добавляем 3
    ret
	
f2:
    fld qword[esp+4]        ; читаем x
    fmul qword[f2_const]    ; умножаем на 2
    fsub qword[f2_const]    ; вычитаем 2
    ret
	
f3:
    fld qword[f3_const]     ; грузим 1
    fld qword[esp+4]        ; читаем x
    fdivp st1,st0           ; считаем 1/x
    ret