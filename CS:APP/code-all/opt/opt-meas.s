	.file	"opt-meas.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
.globl aprod
	.type	aprod, @function
aprod:
.LFB19:
	.file 1 "opt-meas.c"
	.loc 1 12 0
.LVL0:
	.loc 1 16 0
	leal	-2(%rsi), %ecx
	testl	%ecx, %ecx
	jg	.L2
	movl	$0, %edx
.LVL1:
	movsd	.LC0(%rip), %xmm0
.LVL2:
	jmp	.L3
.LVL3:
.L2:
	movl	$0, %edx
.LVL4:
	movsd	.LC0(%rip), %xmm0
.LVL5:
.L4:
	.loc 1 17 0
	movslq	%edx,%rax
	.loc 1 18 0
	mulsd	(%rdi,%rax,8), %xmm0
.LVL6:
	mulsd	8(%rdi,%rax,8), %xmm0
	mulsd	16(%rdi,%rax,8), %xmm0
.LVL7:
	.loc 1 16 0
	addl	$3, %edx
	cmpl	%ecx, %edx
	jl	.L4
.L3:
	.loc 1 20 0
	cmpl	%edx, %esi
	jle	.L5
.L8:
	.loc 1 21 0
	movslq	%edx,%rax
.LVL8:
	mulsd	(%rdi,%rax,8), %xmm0
	.loc 1 20 0
	addl	$1, %edx
.LVL9:
	cmpl	%edx, %esi
	jg	.L8
.L5:
.LVL10:
	.loc 1 23 0
	rep
	ret
.LFE19:
	.size	aprod, .-aprod
.globl aprod1
	.type	aprod1, @function
aprod1:
.LFB20:
	.loc 1 27 0
.LVL11:
	.loc 1 31 0
	testl	%esi, %esi
	jg	.L12
	movsd	.LC0(%rip), %xmm0
.LVL12:
	ret
.LVL13:
.L12:
	movl	$0, %edx
.LVL14:
	movsd	.LC0(%rip), %xmm0
.LVL15:
.L14:
	.loc 1 32 0
	movslq	%edx,%rax
	.loc 1 34 0
	mulsd	(%rdi,%rax,8), %xmm0
.LVL16:
	mulsd	8(%rdi,%rax,8), %xmm0
	mulsd	16(%rdi,%rax,8), %xmm0
.LVL17:
	.loc 1 31 0
	addl	$3, %edx
	cmpl	%edx, %esi
	jg	.L14
.LVL18:
	.loc 1 40 0
	rep
	ret
.LFE20:
	.size	aprod1, .-aprod1
.globl aprod2
	.type	aprod2, @function
aprod2:
.LFB21:
	.loc 1 43 0
.LVL19:
	.loc 1 47 0
	testl	%esi, %esi
	jg	.L18
	movsd	.LC0(%rip), %xmm1
.LVL20:
	jmp	.L19
.LVL21:
.L18:
	movl	$0, %edx
.LVL22:
	movsd	.LC0(%rip), %xmm1
.LVL23:
.L20:
	.loc 1 48 0
	movslq	%edx,%rax
	.loc 1 50 0
	movsd	(%rdi,%rax,8), %xmm0
	mulsd	8(%rdi,%rax,8), %xmm0
	mulsd	%xmm1, %xmm0
	movapd	%xmm0, %xmm1
	mulsd	16(%rdi,%rax,8), %xmm1
	.loc 1 47 0
	addl	$3, %edx
	cmpl	%edx, %esi
	jg	.L20
.L19:
	.loc 1 56 0
	movapd	%xmm1, %xmm0
	ret
.LFE21:
	.size	aprod2, .-aprod2
.globl aprod3
	.type	aprod3, @function
aprod3:
.LFB22:
	.loc 1 59 0
.LVL24:
	.loc 1 63 0
	testl	%esi, %esi
	jg	.L24
	movsd	.LC0(%rip), %xmm1
.LVL25:
	jmp	.L25
.LVL26:
.L24:
	movl	$0, %edx
.LVL27:
	movsd	.LC0(%rip), %xmm1
.LVL28:
.L26:
	.loc 1 64 0
	movslq	%edx,%rax
	.loc 1 66 0
	movsd	(%rdi,%rax,8), %xmm0
	mulsd	8(%rdi,%rax,8), %xmm0
	mulsd	16(%rdi,%rax,8), %xmm0
	mulsd	%xmm0, %xmm1
	.loc 1 63 0
	addl	$3, %edx
	cmpl	%edx, %esi
	jg	.L26
.L25:
	.loc 1 72 0
	movapd	%xmm1, %xmm0
	ret
.LFE22:
	.size	aprod3, .-aprod3
.globl aprod4
	.type	aprod4, @function
aprod4:
.LFB23:
	.loc 1 75 0
.LVL29:
	.loc 1 79 0
	testl	%esi, %esi
	jg	.L30
	movsd	.LC0(%rip), %xmm1
.LVL30:
	jmp	.L31
.LVL31:
.L30:
	movsd	.LC0(%rip), %xmm1
.LVL32:
	movl	$0, %edx
.LVL33:
.L32:
	.loc 1 80 0
	movslq	%edx,%rax
	.loc 1 82 0
	movsd	8(%rdi,%rax,8), %xmm0
	mulsd	16(%rdi,%rax,8), %xmm0
	mulsd	(%rdi,%rax,8), %xmm0
	mulsd	%xmm0, %xmm1
	.loc 1 79 0
	addl	$3, %edx
	cmpl	%edx, %esi
	jg	.L32
.L31:
	.loc 1 88 0
	movapd	%xmm1, %xmm0
	ret
.LFE23:
	.size	aprod4, .-aprod4
.globl aprod5
	.type	aprod5, @function
aprod5:
.LFB24:
	.loc 1 91 0
.LVL34:
	.loc 1 95 0
	testl	%esi, %esi
	jg	.L36
	movsd	.LC0(%rip), %xmm0
.LVL35:
	ret
.LVL36:
.L36:
	movsd	.LC0(%rip), %xmm0
.LVL37:
	movl	$0, %edx
.LVL38:
.L38:
	.loc 1 96 0
	movslq	%edx,%rax
	.loc 1 98 0
	movapd	%xmm0, %xmm1
	mulsd	(%rdi,%rax,8), %xmm1
	movsd	8(%rdi,%rax,8), %xmm0
.LVL39:
	mulsd	16(%rdi,%rax,8), %xmm0
	mulsd	%xmm1, %xmm0
.LVL40:
	.loc 1 95 0
	addl	$3, %edx
	cmpl	%edx, %esi
	jg	.L38
.LVL41:
	.loc 1 104 0
	rep
	ret
.LFE24:
	.size	aprod5, .-aprod5
.globl t1
	.type	t1, @function
t1:
.LFB25:
	.loc 1 112 0
.LVL42:
	.loc 1 113 0
	movl	%edi, %esi
	movl	$a, %edi
.LVL43:
	call	aprod1
	movsd	%xmm0, sink(%rip)
	.loc 1 114 0
	ret
.LFE25:
	.size	t1, .-t1
.globl t2
	.type	t2, @function
t2:
.LFB26:
	.loc 1 117 0
.LVL44:
	.loc 1 118 0
	movl	%edi, %esi
	movl	$a, %edi
.LVL45:
	call	aprod2
	movsd	%xmm0, sink(%rip)
	.loc 1 119 0
	ret
.LFE26:
	.size	t2, .-t2
.globl t3
	.type	t3, @function
t3:
.LFB27:
	.loc 1 122 0
.LVL46:
	.loc 1 123 0
	movl	%edi, %esi
	movl	$a, %edi
.LVL47:
	call	aprod3
	movsd	%xmm0, sink(%rip)
	.loc 1 124 0
	ret
.LFE27:
	.size	t3, .-t3
.globl t4
	.type	t4, @function
t4:
.LFB28:
	.loc 1 127 0
.LVL48:
	.loc 1 128 0
	movl	%edi, %esi
	movl	$a, %edi
.LVL49:
	call	aprod4
	movsd	%xmm0, sink(%rip)
	.loc 1 129 0
	ret
.LFE28:
	.size	t4, .-t4
.globl t5
	.type	t5, @function
t5:
.LFB29:
	.loc 1 132 0
.LVL50:
	.loc 1 133 0
	movl	%edi, %esi
	movl	$a, %edi
.LVL51:
	call	aprod5
	movsd	%xmm0, sink(%rip)
	.loc 1 134 0
	ret
.LFE29:
	.size	t5, .-t5
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"A1 CPE = %.2f\n"
.LC2:
	.string	"A2 CPE = %.2f\n"
.LC3:
	.string	"A3 CPE = %.2f\n"
.LC4:
	.string	"A4 CPE = %.2f\n"
.LC5:
	.string	"A5 CPE = %.2f\n"
	.text
.globl main
	.type	main, @function
main:
.LFB30:
	.loc 1 136 0
	subq	$8, %rsp
.LCFI0:
	.loc 1 137 0
	movl	$1024, %esi
	movl	$t1, %edi
	call	find_cpe
.LBB12:
.LBB13:
	.file 2 "/usr/include/bits/stdio2.h"
	.loc 2 105 0
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE13:
.LBE12:
	.loc 1 138 0
	movl	$1024, %esi
	movl	$t2, %edi
	call	find_cpe
.LBB14:
.LBB15:
	.loc 2 105 0
	movl	$.LC2, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE15:
.LBE14:
	.loc 1 139 0
	movl	$1024, %esi
	movl	$t3, %edi
	call	find_cpe
.LBB16:
.LBB17:
	.loc 2 105 0
	movl	$.LC3, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE17:
.LBE16:
	.loc 1 140 0
	movl	$1024, %esi
	movl	$t4, %edi
	call	find_cpe
.LBB18:
.LBB19:
	.loc 2 105 0
	movl	$.LC4, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE19:
.LBE18:
	.loc 1 141 0
	movl	$1024, %esi
	movl	$t5, %edi
	call	find_cpe
.LBB20:
.LBB21:
	.loc 2 105 0
	movl	$.LC5, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE21:
.LBE20:
	.loc 1 143 0
	movl	$0, %eax
	addq	$8, %rsp
	ret
.LFE30:
	.size	main, .-main
.globl sink
	.bss
	.align 8
	.type	sink, @object
	.size	sink, 8
sink:
	.zero	8
	.comm	a,8192,32
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.section	.debug_frame,"",@progbits
.Lframe0:
	.long	.LECIE0-.LSCIE0
.LSCIE0:
	.long	0xffffffff
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE0:
.LSFDE0:
	.long	.LEFDE0-.LASFDE0
.LASFDE0:
	.long	.Lframe0
	.quad	.LFB19
	.quad	.LFE19-.LFB19
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB20
	.quad	.LFE20-.LFB20
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB21
	.quad	.LFE21-.LFB21
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB23
	.quad	.LFE23-.LFB23
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB25
	.quad	.LFE25-.LFB25
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.align 8
.LEFDE14:
.LSFDE16:
	.long	.LEFDE16-.LASFDE16
.LASFDE16:
	.long	.Lframe0
	.quad	.LFB27
	.quad	.LFE27-.LFB27
	.align 8
.LEFDE16:
.LSFDE18:
	.long	.LEFDE18-.LASFDE18
.LASFDE18:
	.long	.Lframe0
	.quad	.LFB28
	.quad	.LFE28-.LFB28
	.align 8
.LEFDE18:
.LSFDE20:
	.long	.LEFDE20-.LASFDE20
.LASFDE20:
	.long	.Lframe0
	.quad	.LFB29
	.quad	.LFE29-.LFB29
	.align 8
.LEFDE20:
.LSFDE22:
	.long	.LEFDE22-.LASFDE22
.LASFDE22:
	.long	.Lframe0
	.quad	.LFB30
	.quad	.LFE30-.LFB30
	.byte	0x4
	.long	.LCFI0-.LFB30
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE22:
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB30
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE23:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST1:
	.quad	.LVL1-.Ltext0
	.quad	.LVL3-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL4-.Ltext0
	.quad	.LVL8-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL8-.Ltext0
	.quad	.LVL9-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL9-.Ltext0
	.quad	.LFE19-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST2:
	.quad	.LVL2-.Ltext0
	.quad	.LVL3-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL5-.Ltext0
	.quad	.LVL6-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL7-.Ltext0
	.quad	.LVL10-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST4:
	.quad	.LVL12-.Ltext0
	.quad	.LVL13-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL15-.Ltext0
	.quad	.LVL16-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL17-.Ltext0
	.quad	.LVL18-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST6:
	.quad	.LVL20-.Ltext0
	.quad	.LVL21-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	.LVL23-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	0x0
	.quad	0x0
.LLST8:
	.quad	.LVL25-.Ltext0
	.quad	.LVL26-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	.LVL28-.Ltext0
	.quad	.LFE22-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	0x0
	.quad	0x0
.LLST10:
	.quad	.LVL30-.Ltext0
	.quad	.LVL31-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	.LVL32-.Ltext0
	.quad	.LFE23-.Ltext0
	.value	0x1
	.byte	0x62
	.quad	0x0
	.quad	0x0
.LLST12:
	.quad	.LVL35-.Ltext0
	.quad	.LVL36-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL37-.Ltext0
	.quad	.LVL39-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL40-.Ltext0
	.quad	.LVL41-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	0x0
	.quad	0x0
.LLST14:
	.quad	.LVL42-.Ltext0
	.quad	.LVL43-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST16:
	.quad	.LVL44-.Ltext0
	.quad	.LVL45-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL46-.Ltext0
	.quad	.LVL47-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST20:
	.quad	.LVL48-.Ltext0
	.quad	.LVL49-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST22:
	.quad	.LVL50-.Ltext0
	.quad	.LVL51-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST23:
	.quad	.LFB30-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LFE30-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
	.section	.debug_info
	.long	0x52e
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF19
	.byte	0x1
	.long	.LASF20
	.long	.LASF21
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x4
	.byte	0x8
	.byte	0x7
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x5
	.byte	0x8
	.long	0x75
	.uleb128 0x6
	.long	0x68
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x7
	.byte	0x1
	.long	.LASF22
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.long	0x57
	.byte	0x2
	.long	0xa7
	.uleb128 0x8
	.long	.LASF23
	.byte	0x2
	.byte	0x67
	.long	0x6f
	.uleb128 0x9
	.byte	0x0
	.uleb128 0xa
	.byte	0x1
	.long	.LASF11
	.byte	0x1
	.byte	0xc
	.byte	0x1
	.long	0x117
	.quad	.LFB19
	.quad	.LFE19
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x117
	.uleb128 0xb
	.string	"a"
	.byte	0x1
	.byte	0xb
	.long	0x11e
	.byte	0x1
	.byte	0x55
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0xb
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0xd
	.long	0x57
	.long	.LLST1
	.uleb128 0xd
	.string	"x"
	.byte	0x1
	.byte	0xe
	.long	0x117
	.uleb128 0xd
	.string	"y"
	.byte	0x1
	.byte	0xe
	.long	0x117
	.uleb128 0xd
	.string	"z"
	.byte	0x1
	.byte	0xe
	.long	0x117
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0xf
	.long	0x117
	.long	.LLST2
	.byte	0x0
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF10
	.uleb128 0x5
	.byte	0x8
	.long	0x117
	.uleb128 0xa
	.byte	0x1
	.long	.LASF12
	.byte	0x1
	.byte	0x1b
	.byte	0x1
	.long	0x117
	.quad	.LFB20
	.quad	.LFE20
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x192
	.uleb128 0xb
	.string	"a"
	.byte	0x1
	.byte	0x1a
	.long	0x11e
	.byte	0x1
	.byte	0x55
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0x1a
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x1c
	.long	0x57
	.byte	0x1
	.byte	0x51
	.uleb128 0xd
	.string	"x"
	.byte	0x1
	.byte	0x1d
	.long	0x117
	.uleb128 0xd
	.string	"y"
	.byte	0x1
	.byte	0x1d
	.long	0x117
	.uleb128 0xd
	.string	"z"
	.byte	0x1
	.byte	0x1d
	.long	0x117
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x1e
	.long	0x117
	.long	.LLST4
	.byte	0x0
	.uleb128 0xa
	.byte	0x1
	.long	.LASF13
	.byte	0x1
	.byte	0x2b
	.byte	0x1
	.long	0x117
	.quad	.LFB21
	.quad	.LFE21
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x200
	.uleb128 0xb
	.string	"a"
	.byte	0x1
	.byte	0x2a
	.long	0x11e
	.byte	0x1
	.byte	0x55
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0x2a
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x2c
	.long	0x57
	.byte	0x1
	.byte	0x51
	.uleb128 0xd
	.string	"x"
	.byte	0x1
	.byte	0x2d
	.long	0x117
	.uleb128 0xd
	.string	"y"
	.byte	0x1
	.byte	0x2d
	.long	0x117
	.uleb128 0xd
	.string	"z"
	.byte	0x1
	.byte	0x2d
	.long	0x117
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x2e
	.long	0x117
	.long	.LLST6
	.byte	0x0
	.uleb128 0xa
	.byte	0x1
	.long	.LASF14
	.byte	0x1
	.byte	0x3b
	.byte	0x1
	.long	0x117
	.quad	.LFB22
	.quad	.LFE22
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x26e
	.uleb128 0xb
	.string	"a"
	.byte	0x1
	.byte	0x3a
	.long	0x11e
	.byte	0x1
	.byte	0x55
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0x3a
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x3c
	.long	0x57
	.byte	0x1
	.byte	0x51
	.uleb128 0xd
	.string	"x"
	.byte	0x1
	.byte	0x3d
	.long	0x117
	.uleb128 0xd
	.string	"y"
	.byte	0x1
	.byte	0x3d
	.long	0x117
	.uleb128 0xd
	.string	"z"
	.byte	0x1
	.byte	0x3d
	.long	0x117
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x3e
	.long	0x117
	.long	.LLST8
	.byte	0x0
	.uleb128 0xa
	.byte	0x1
	.long	.LASF15
	.byte	0x1
	.byte	0x4b
	.byte	0x1
	.long	0x117
	.quad	.LFB23
	.quad	.LFE23
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x2dc
	.uleb128 0xb
	.string	"a"
	.byte	0x1
	.byte	0x4a
	.long	0x11e
	.byte	0x1
	.byte	0x55
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0x4a
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x4c
	.long	0x57
	.byte	0x1
	.byte	0x51
	.uleb128 0xd
	.string	"x"
	.byte	0x1
	.byte	0x4d
	.long	0x117
	.uleb128 0xd
	.string	"y"
	.byte	0x1
	.byte	0x4d
	.long	0x117
	.uleb128 0xd
	.string	"z"
	.byte	0x1
	.byte	0x4d
	.long	0x117
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x4e
	.long	0x117
	.long	.LLST10
	.byte	0x0
	.uleb128 0xa
	.byte	0x1
	.long	.LASF16
	.byte	0x1
	.byte	0x5b
	.byte	0x1
	.long	0x117
	.quad	.LFB24
	.quad	.LFE24
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x34a
	.uleb128 0xb
	.string	"a"
	.byte	0x1
	.byte	0x5a
	.long	0x11e
	.byte	0x1
	.byte	0x55
	.uleb128 0xb
	.string	"n"
	.byte	0x1
	.byte	0x5a
	.long	0x57
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x5c
	.long	0x57
	.byte	0x1
	.byte	0x51
	.uleb128 0xd
	.string	"x"
	.byte	0x1
	.byte	0x5d
	.long	0x117
	.uleb128 0xd
	.string	"y"
	.byte	0x1
	.byte	0x5d
	.long	0x117
	.uleb128 0xd
	.string	"z"
	.byte	0x1
	.byte	0x5d
	.long	0x117
	.uleb128 0xc
	.string	"r"
	.byte	0x1
	.byte	0x5e
	.long	0x117
	.long	.LLST12
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.string	"t1"
	.byte	0x1
	.byte	0x70
	.byte	0x1
	.quad	.LFB25
	.quad	.LFE25
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x377
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x6f
	.long	0x57
	.long	.LLST14
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.string	"t2"
	.byte	0x1
	.byte	0x75
	.byte	0x1
	.quad	.LFB26
	.quad	.LFE26
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x3a4
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x74
	.long	0x57
	.long	.LLST16
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.string	"t3"
	.byte	0x1
	.byte	0x7a
	.byte	0x1
	.quad	.LFB27
	.quad	.LFE27
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x3d1
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x79
	.long	0x57
	.long	.LLST18
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.string	"t4"
	.byte	0x1
	.byte	0x7f
	.byte	0x1
	.quad	.LFB28
	.quad	.LFE28
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x3fe
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x7e
	.long	0x57
	.long	.LLST20
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.string	"t5"
	.byte	0x1
	.byte	0x84
	.byte	0x1
	.quad	.LFB29
	.quad	.LFE29
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x42b
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x83
	.long	0x57
	.long	.LLST22
	.byte	0x0
	.uleb128 0x11
	.byte	0x1
	.long	.LASF17
	.byte	0x1
	.byte	0x88
	.long	0x57
	.quad	.LFB30
	.quad	.LFE30
	.long	.LLST23
	.long	0x4f1
	.uleb128 0x12
	.long	0x88
	.quad	.LBB12
	.quad	.LBE12
	.byte	0x1
	.byte	0x89
	.long	0x470
	.uleb128 0x13
	.long	0x9a
	.byte	0x0
	.uleb128 0x12
	.long	0x88
	.quad	.LBB14
	.quad	.LBE14
	.byte	0x1
	.byte	0x8a
	.long	0x491
	.uleb128 0x13
	.long	0x9a
	.byte	0x0
	.uleb128 0x12
	.long	0x88
	.quad	.LBB16
	.quad	.LBE16
	.byte	0x1
	.byte	0x8b
	.long	0x4b2
	.uleb128 0x13
	.long	0x9a
	.byte	0x0
	.uleb128 0x12
	.long	0x88
	.quad	.LBB18
	.quad	.LBE18
	.byte	0x1
	.byte	0x8c
	.long	0x4d3
	.uleb128 0x13
	.long	0x9a
	.byte	0x0
	.uleb128 0x14
	.long	0x88
	.quad	.LBB20
	.quad	.LBE20
	.byte	0x1
	.byte	0x8d
	.uleb128 0x13
	.long	0x9a
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.long	.LASF18
	.byte	0x1
	.byte	0x6a
	.long	0x507
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	sink
	.uleb128 0x16
	.long	0x117
	.uleb128 0x17
	.long	0x117
	.long	0x51d
	.uleb128 0x18
	.long	0x65
	.value	0x3ff
	.byte	0x0
	.uleb128 0x19
	.string	"a"
	.byte	0x1
	.byte	0x6d
	.long	0x50c
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	a
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0x35
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x8a
	.value	0x2
	.long	.Ldebug_info0
	.long	0x532
	.long	0xa7
	.string	"aprod"
	.long	0x124
	.string	"aprod1"
	.long	0x192
	.string	"aprod2"
	.long	0x200
	.string	"aprod3"
	.long	0x26e
	.string	"aprod4"
	.long	0x2dc
	.string	"aprod5"
	.long	0x34a
	.string	"t1"
	.long	0x377
	.string	"t2"
	.long	0x3a4
	.string	"t3"
	.long	0x3d1
	.string	"t4"
	.long	0x3fe
	.string	"t5"
	.long	0x42b
	.string	"main"
	.long	0x4f1
	.string	"sink"
	.long	0x51d
	.string	"a"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0x0
	.value	0x0
	.value	0x0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF12:
	.string	"aprod1"
.LASF14:
	.string	"aprod3"
.LASF15:
	.string	"aprod4"
.LASF16:
	.string	"aprod5"
.LASF11:
	.string	"aprod"
.LASF18:
	.string	"sink"
.LASF1:
	.string	"unsigned char"
.LASF0:
	.string	"long unsigned int"
.LASF13:
	.string	"aprod2"
.LASF2:
	.string	"short unsigned int"
.LASF19:
	.string	"GNU C 4.3.2"
.LASF20:
	.string	"opt-meas.c"
.LASF10:
	.string	"double"
.LASF17:
	.string	"main"
.LASF3:
	.string	"unsigned int"
.LASF8:
	.string	"long long unsigned int"
.LASF21:
	.string	"/home/rbryant/ics2/code/opt"
.LASF9:
	.string	"long long int"
.LASF7:
	.string	"char"
.LASF22:
	.string	"printf"
.LASF5:
	.string	"short int"
.LASF6:
	.string	"long int"
.LASF4:
	.string	"signed char"
.LASF23:
	.string	"__fmt"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
