	.file	"lower.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
	.type	init, @function
init:
.LFB29:
	.file 1 "lower.c"
	.loc 1 24 0
	.loc 1 24 0
	movl	$data, %esi
	movl	$0, %ecx
.LVL0:
	.loc 1 27 0
	movl	$1321528399, %edi
.L2:
	movl	%ecx, %eax
	imull	%edi
	sarl	$3, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	imull	$26, %edx, %edx
	movl	%ecx, %eax
	subl	%edx, %eax
	addl	$97, %eax
	movb	%al, (%rsi)
	.loc 1 26 0
	addl	$1, %ecx
	addq	$1, %rsi
	cmpl	$1048576, %ecx
	jne	.L2
	.loc 1 28 0
	rep
	ret
.LFE29:
	.size	init, .-init
	.type	set_len, @function
set_len:
.LFB30:
	.loc 1 31 0
.LVL1:
	.loc 1 32 0
	movslq	%edi,%rdi
.LVL2:
	movb	$0, data(%rdi)
	.loc 1 33 0
	ret
.LFE30:
	.size	set_len, .-set_len
	.type	unset_len, @function
unset_len:
.LFB31:
	.loc 1 36 0
.LVL3:
	.loc 1 37 0
	movslq	%edi,%rcx
.LVL4:
	movl	$1321528399, %edx
	movl	%edi, %eax
	imull	%edx
	sarl	$3, %edx
	movl	%edi, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	imull	$26, %edx, %edx
	subl	%edx, %edi
	addl	$97, %edi
	movb	%dil, data(%rcx)
	.loc 1 38 0
	ret
.LFE31:
	.size	unset_len, .-unset_len
.globl strlen
	.type	strlen, @function
strlen:
.LFB34:
	.loc 1 67 0
.LVL5:
	.loc 1 67 0
	movq	%rdi, %rdx
	movl	$0, %eax
.LVL6:
	cmpb	$0, (%rdi)
	je	.L11
.L14:
	.loc 1 68 0
	addq	$1, %rdx
.LVL7:
	movl	%edx, %eax
.LVL8:
	subl	%edi, %eax
.LVL9:
	.loc 1 67 0
	cmpb	$0, (%rdx)
	jne	.L14
.LVL10:
.L11:
	cltq
.LVL11:
.LVL12:
	.loc 1 72 0
	ret
.LFE34:
	.size	strlen, .-strlen
.globl lower2
	.type	lower2, @function
lower2:
.LFB33:
	.loc 1 53 0
.LVL13:
	movq	%rdi, %r8
	.loc 1 55 0
	movl	$0, %eax
	movq	$-1, %rcx
	repnz scasb
.LVL14:
	notq	%rcx
	subl	$1, %ecx
.LVL15:
	.loc 1 57 0
	testl	%ecx, %ecx
	jle	.L20
	movl	$0, %edx
.LVL16:
.L19:
	.loc 1 58 0
	movzbl	(%r8,%rdx), %esi
	leal	-65(%rsi), %eax
	cmpb	$25, %al
	ja	.L18
	.loc 1 59 0
	leal	32(%rsi), %eax
	movb	%al, (%r8,%rdx)
.L18:
	addq	$1, %rdx
	.loc 1 57 0
	cmpl	%edx, %ecx
	jg	.L19
.L20:
	rep
	ret
.LFE33:
	.size	lower2, .-lower2
.globl lower1
	.type	lower1, @function
lower1:
.LFB32:
	.loc 1 43 0
.LVL17:
	pushq	%rbx
.LCFI0:
	movq	%rdi, %r8
	.loc 1 43 0
	movl	$0, %esi
.LVL18:
	.loc 1 46 0
	movq	$-1, %r10
	movq	%rdi, %rbx
	movl	$0, %r9d
	jmp	.L23
.LVL19:
.L25:
	.loc 1 47 0
	leaq	(%r8,%r11), %rcx
	movzbl	(%rcx), %edx
	leal	-65(%rdx), %eax
	cmpb	$25, %al
	ja	.L24
.LVL20:
	.loc 1 48 0
	leal	32(%rdx), %eax
	movb	%al, (%rcx)
.L24:
	.loc 1 46 0
	addl	$1, %esi
.LVL21:
.L23:
	movslq	%esi,%r11
	movq	%r10, %rcx
	movq	%rbx, %rdi
	movl	%r9d, %eax
	repnz scasb
	notq	%rcx
	subq	$1, %rcx
	cmpq	%rcx, %r11
	jb	.L25
	.loc 1 49 0
	popq	%rbx
.LVL22:
	ret
.LFE32:
	.size	lower1, .-lower1
.globl test_lower1
	.type	test_lower1, @function
test_lower1:
.LFB35:
	.loc 1 76 0
.LVL23:
	pushq	%rbx
.LCFI1:
	.loc 1 77 0
	movl	(%rdi), %ebx
.LVL24:
	.loc 1 78 0
	movl	%ebx, %edi
.LVL25:
	call	set_len
	.loc 1 79 0
	movl	$data, %edi
	call	lower1
	.loc 1 80 0
	movl	%ebx, %edi
	call	unset_len
	.loc 1 81 0
	popq	%rbx
.LVL26:
	ret
.LFE35:
	.size	test_lower1, .-test_lower1
.globl test_lower2
	.type	test_lower2, @function
test_lower2:
.LFB36:
	.loc 1 84 0
.LVL27:
	pushq	%rbx
.LCFI2:
	.loc 1 85 0
	movl	(%rdi), %ebx
.LVL28:
	.loc 1 86 0
	movl	%ebx, %edi
.LVL29:
	call	set_len
	.loc 1 87 0
	movl	$data, %edi
	call	lower2
	.loc 1 88 0
	movl	%ebx, %edi
	call	unset_len
	.loc 1 89 0
	popq	%rbx
.LVL30:
	ret
.LFE36:
	.size	test_lower2, .-test_lower2
	.type	run_test, @function
run_test:
.LFB37:
	.loc 1 93 0
.LVL31:
	pushq	%rbx
.LCFI3:
	subq	$16, %rsp
.LCFI4:
	movq	%rdi, %rbx
	movl	%esi, 12(%rsp)
	.loc 1 97 0
	movl	$0, %edi
.LVL32:
	call	set_fcyc_compensate
.LVL33:
	.loc 1 98 0
	leaq	12(%rsp), %rsi
	movq	%rbx, %rdi
	call	fcyc
	movsd	.LC0(%rip), %xmm1
	mulsd	clock_rate(%rip), %xmm1
	.loc 1 101 0
	divsd	%xmm1, %xmm0
	addq	$16, %rsp
	popq	%rbx
.LVL34:
	ret
.LFE37:
	.size	run_test, .-run_test
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"& %d "
.LC2:
	.string	" \\\\"
.LC3:
	.string	"& %.2f "
.LC4:
	.string	"& %.4f "
	.text
.globl expo
	.type	expo, @function
expo:
.LFB39:
	.loc 1 117 0
	pushq	%rbx
.LCFI5:
	.loc 1 117 0
	movl	$16384, %ebx
.LVL35:
.L34:
.LBB14:
.LBB15:
	.file 2 "/usr/include/bits/stdio2.h"
	.loc 2 105 0
	movl	%ebx, %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
.LBE15:
.LBE14:
	.loc 1 119 0
	addl	%ebx, %ebx
	cmpl	$1048576, %ebx
	jle	.L34
	.loc 1 121 0
	movl	$.LC2, %edi
	call	puts
	movl	$16384, %ebx
.LVL36:
.L35:
	.loc 1 123 0
	movl	%ebx, %esi
	movl	$test_lower1, %edi
	call	run_test
.LBB16:
.LBB17:
	.loc 2 105 0
	movl	$.LC3, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE17:
.LBE16:
	.loc 1 122 0
	addl	%ebx, %ebx
	cmpl	$1048576, %ebx
	jle	.L35
	.loc 1 124 0
	movl	$.LC2, %edi
	call	puts
	movl	$16384, %ebx
.LVL37:
.L36:
	.loc 1 126 0
	movl	%ebx, %esi
	movl	$test_lower2, %edi
	call	run_test
.LBB18:
.LBB19:
	.loc 2 105 0
	movl	$.LC4, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE19:
.LBE18:
	.loc 1 125 0
	addl	%ebx, %ebx
	cmpl	$1048576, %ebx
	jle	.L36
	.loc 1 127 0
	movl	$.LC2, %edi
	call	puts
	.loc 1 128 0
	popq	%rbx
.LVL38:
	ret
.LFE39:
	.size	expo, .-expo
	.section	.rodata.str1.1
.LC5:
	.string	"Length\tlower1\tlower2"
.LC6:
	.string	"%d"
.LC7:
	.string	"\t%f"
.LC8:
	.string	"\t%f\n"
	.text
.globl linear
	.type	linear, @function
linear:
.LFB38:
	.loc 1 106 0
	pushq	%rbx
.LCFI6:
	.loc 1 108 0
	movl	$.LC5, %edi
	call	puts
	movl	$20000, %ebx
.LVL39:
.L42:
.LBB20:
.LBB21:
	.loc 2 105 0
	movl	%ebx, %edx
	movl	$.LC6, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
.LBE21:
.LBE20:
	.loc 1 111 0
	movl	%ebx, %esi
	movl	$test_lower1, %edi
	call	run_test
.LBB22:
.LBB23:
	.loc 2 105 0
	movl	$.LC7, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE23:
.LBE22:
	.loc 1 112 0
	movl	%ebx, %esi
	movl	$test_lower2, %edi
	call	run_test
.LBB24:
.LBB25:
	.loc 2 105 0
	movl	$.LC8, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE25:
.LBE24:
	.loc 1 109 0
	addl	$20000, %ebx
	cmpl	$520000, %ebx
	jne	.L42
	.loc 1 114 0
	popq	%rbx
.LVL40:
	ret
.LFE38:
	.size	linear, .-linear
.globl main
	.type	main, @function
main:
.LFB40:
	.loc 1 131 0
.LVL41:
	pushq	%rbx
.LCFI7:
	movl	%edi, %ebx
	.loc 1 132 0
	movl	$0, %eax
	call	init
.LVL42:
	.loc 1 133 0
	movl	$1, %edi
	call	mhz
	movsd	%xmm0, clock_rate(%rip)
	.loc 1 134 0
	cmpl	$1, %ebx
	jle	.L46
	.loc 1 135 0
	movl	$0, %eax
	call	expo
	jmp	.L47
.L46:
	.loc 1 137 0
	movl	$0, %eax
	call	linear
.L47:
	.loc 1 139 0
	movl	$0, %eax
	popq	%rbx
.LVL43:
	.p2align 4,,3
	.p2align 3
	ret
.LFE40:
	.size	main, .-main
	.local	clock_rate
	.comm	clock_rate,8,8
	.local	data
	.comm	data,1048576,32
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1093567616
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
	.quad	.LFB29
	.quad	.LFE29-.LFB29
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB30
	.quad	.LFE30-.LFB30
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB31
	.quad	.LFE31-.LFB31
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB34
	.quad	.LFE34-.LFB34
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB33
	.quad	.LFE33-.LFB33
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB32
	.quad	.LFE32-.LFB32
	.byte	0x4
	.long	.LCFI0-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB35
	.quad	.LFE35-.LFB35
	.byte	0x4
	.long	.LCFI1-.LFB35
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB36
	.quad	.LFE36-.LFB36
	.byte	0x4
	.long	.LCFI2-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE14:
.LSFDE16:
	.long	.LEFDE16-.LASFDE16
.LASFDE16:
	.long	.Lframe0
	.quad	.LFB37
	.quad	.LFE37-.LFB37
	.byte	0x4
	.long	.LCFI3-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE16:
.LSFDE18:
	.long	.LEFDE18-.LASFDE18
.LASFDE18:
	.long	.Lframe0
	.quad	.LFB39
	.quad	.LFE39-.LFB39
	.byte	0x4
	.long	.LCFI5-.LFB39
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE18:
.LSFDE20:
	.long	.LEFDE20-.LASFDE20
.LASFDE20:
	.long	.Lframe0
	.quad	.LFB38
	.quad	.LFE38-.LFB38
	.byte	0x4
	.long	.LCFI6-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE20:
.LSFDE22:
	.long	.LEFDE22-.LASFDE22
.LASFDE22:
	.long	.Lframe0
	.quad	.LFB40
	.quad	.LFE40-.LFB40
	.byte	0x4
	.long	.LCFI7-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
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
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB35
	.long	.LFE35-.LFB35
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB35
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB39
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE23:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST3:
	.quad	.LVL3-.Ltext0
	.quad	.LVL4-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL4-.Ltext0
	.quad	.LFE31-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST5:
	.quad	.LVL5-.Ltext0
	.quad	.LVL7-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL7-.Ltext0
	.quad	.LVL8-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL10-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST6:
	.quad	.LVL6-.Ltext0
	.quad	.LVL12-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST8:
	.quad	.LVL13-.Ltext0
	.quad	.LVL14-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL16-.Ltext0
	.quad	.LFE33-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST9:
	.quad	.LFB32-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST10:
	.quad	.LVL17-.Ltext0
	.quad	.LVL19-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL19-.Ltext0
	.quad	.LVL20-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL20-.Ltext0
	.quad	.LVL21-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL21-.Ltext0
	.quad	.LVL22-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL22-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST11:
	.quad	.LFB35-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI1-.Ltext0
	.quad	.LFE35-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST12:
	.quad	.LVL23-.Ltext0
	.quad	.LVL25-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST13:
	.quad	.LVL24-.Ltext0
	.quad	.LVL26-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST14:
	.quad	.LFB36-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI2-.Ltext0
	.quad	.LFE36-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST15:
	.quad	.LVL27-.Ltext0
	.quad	.LVL29-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST16:
	.quad	.LVL28-.Ltext0
	.quad	.LVL30-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST17:
	.quad	.LFB37-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI3-.Ltext0
	.quad	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI4-.Ltext0
	.quad	.LFE37-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL31-.Ltext0
	.quad	.LVL32-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL32-.Ltext0
	.quad	.LVL34-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST19:
	.quad	.LVL31-.Ltext0
	.quad	.LVL33-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL33-.Ltext0
	.quad	.LFE37-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 12
	.quad	0x0
	.quad	0x0
.LLST20:
	.quad	.LFB39-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI5-.Ltext0
	.quad	.LFE39-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST21:
	.quad	.LVL35-.Ltext0
	.quad	.LVL38-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST22:
	.quad	.LFB38-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI6-.Ltext0
	.quad	.LFE38-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST23:
	.quad	.LVL39-.Ltext0
	.quad	.LVL40-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST24:
	.quad	.LFB40-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI7-.Ltext0
	.quad	.LFE40-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST25:
	.quad	.LVL41-.Ltext0
	.quad	.LVL42-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL42-.Ltext0
	.quad	.LVL43-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST26:
	.quad	.LVL41-.Ltext0
	.quad	.LVL42-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
	.file 3 "/usr/lib/gcc/x86_64-linux-gnu/4.3.2/include/stddef.h"
	.file 4 "../include/fcyc.h"
	.section	.debug_info
	.long	0x4a2
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF33
	.byte	0x1
	.long	.LASF34
	.long	.LASF35
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF10
	.byte	0x3
	.byte	0xd6
	.long	0x38
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x5
	.byte	0x8
	.byte	0x7
	.uleb128 0x6
	.byte	0x8
	.long	0x79
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x6
	.byte	0x8
	.long	0x86
	.uleb128 0x7
	.long	0x79
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x2
	.long	.LASF11
	.byte	0x4
	.byte	0x7
	.long	0xa4
	.uleb128 0x6
	.byte	0x8
	.long	0xaa
	.uleb128 0x8
	.byte	0x1
	.long	0xb6
	.uleb128 0x9
	.long	0xb6
	.byte	0x0
	.uleb128 0x6
	.byte	0x8
	.long	0x62
	.uleb128 0xa
	.byte	0x1
	.long	.LASF23
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.long	0x62
	.byte	0x2
	.long	0xdb
	.uleb128 0xb
	.long	.LASF36
	.byte	0x2
	.byte	0x67
	.long	0x80
	.uleb128 0xc
	.byte	0x0
	.uleb128 0xd
	.long	.LASF37
	.byte	0x1
	.byte	0x18
	.quad	.LFB29
	.quad	.LFE29
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x105
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x19
	.long	0x62
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xf
	.long	.LASF12
	.byte	0x1
	.byte	0x1f
	.byte	0x1
	.quad	.LFB30
	.quad	.LFE30
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x132
	.uleb128 0x10
	.string	"len"
	.byte	0x1
	.byte	0x1e
	.long	0x62
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0xf
	.long	.LASF13
	.byte	0x1
	.byte	0x24
	.byte	0x1
	.quad	.LFB31
	.quad	.LFE31
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x161
	.uleb128 0x11
	.string	"len"
	.byte	0x1
	.byte	0x23
	.long	0x62
	.long	.LLST3
	.byte	0x0
	.uleb128 0x12
	.byte	0x1
	.long	.LASF27
	.byte	0x1
	.byte	0x41
	.byte	0x1
	.long	0x2d
	.quad	.LFB34
	.quad	.LFE34
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x1a2
	.uleb128 0x11
	.string	"s"
	.byte	0x1
	.byte	0x40
	.long	0x80
	.long	.LLST5
	.uleb128 0x13
	.long	.LASF14
	.byte	0x1
	.byte	0x42
	.long	0x62
	.long	.LLST6
	.byte	0x0
	.uleb128 0x14
	.byte	0x1
	.long	.LASF15
	.byte	0x1
	.byte	0x35
	.byte	0x1
	.quad	.LFB33
	.quad	.LFE33
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x1e6
	.uleb128 0x11
	.string	"s"
	.byte	0x1
	.byte	0x34
	.long	0x73
	.long	.LLST8
	.uleb128 0x15
	.string	"i"
	.byte	0x1
	.byte	0x36
	.long	0x62
	.uleb128 0xe
	.string	"len"
	.byte	0x1
	.byte	0x37
	.long	0x62
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF16
	.byte	0x1
	.byte	0x2b
	.byte	0x1
	.quad	.LFB32
	.quad	.LFE32
	.long	.LLST9
	.long	0x220
	.uleb128 0x11
	.string	"s"
	.byte	0x1
	.byte	0x2a
	.long	0x73
	.long	.LLST10
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x2c
	.long	0x62
	.byte	0x1
	.byte	0x54
	.byte	0x0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF17
	.byte	0x1
	.byte	0x4c
	.byte	0x1
	.quad	.LFB35
	.quad	.LFE35
	.long	.LLST11
	.long	0x260
	.uleb128 0x17
	.long	.LASF18
	.byte	0x1
	.byte	0x4b
	.long	0xb6
	.long	.LLST12
	.uleb128 0x18
	.string	"len"
	.byte	0x1
	.byte	0x4d
	.long	0x62
	.long	.LLST13
	.byte	0x0
	.uleb128 0x16
	.byte	0x1
	.long	.LASF19
	.byte	0x1
	.byte	0x54
	.byte	0x1
	.quad	.LFB36
	.quad	.LFE36
	.long	.LLST14
	.long	0x2a0
	.uleb128 0x17
	.long	.LASF18
	.byte	0x1
	.byte	0x53
	.long	0xb6
	.long	.LLST15
	.uleb128 0x18
	.string	"len"
	.byte	0x1
	.byte	0x55
	.long	0x62
	.long	.LLST16
	.byte	0x0
	.uleb128 0x19
	.long	.LASF38
	.byte	0x1
	.byte	0x5d
	.byte	0x1
	.long	0x2f9
	.quad	.LFB37
	.quad	.LFE37
	.long	.LLST17
	.long	0x2f9
	.uleb128 0x11
	.string	"fun"
	.byte	0x1
	.byte	0x5c
	.long	0x99
	.long	.LLST18
	.uleb128 0x11
	.string	"len"
	.byte	0x1
	.byte	0x5c
	.long	0x62
	.long	.LLST19
	.uleb128 0x1a
	.long	.LASF20
	.byte	0x1
	.byte	0x5e
	.long	0x2f9
	.uleb128 0x1a
	.long	.LASF21
	.byte	0x1
	.byte	0x5f
	.long	0x2f9
	.byte	0x0
	.uleb128 0x3
	.byte	0x8
	.byte	0x4
	.long	.LASF22
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF24
	.byte	0x1
	.byte	0x75
	.quad	.LFB39
	.quad	.LFE39
	.long	.LLST20
	.long	0x38f
	.uleb128 0x13
	.long	.LASF25
	.byte	0x1
	.byte	0x76
	.long	0x62
	.long	.LLST21
	.uleb128 0x1c
	.long	0xbc
	.quad	.LBB14
	.quad	.LBE14
	.byte	0x1
	.byte	0x78
	.long	0x350
	.uleb128 0x1d
	.long	0xce
	.byte	0x0
	.uleb128 0x1c
	.long	0xbc
	.quad	.LBB16
	.quad	.LBE16
	.byte	0x1
	.byte	0x7b
	.long	0x371
	.uleb128 0x1d
	.long	0xce
	.byte	0x0
	.uleb128 0x1e
	.long	0xbc
	.quad	.LBB18
	.quad	.LBE18
	.byte	0x1
	.byte	0x7e
	.uleb128 0x1d
	.long	0xce
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF26
	.byte	0x1
	.byte	0x6a
	.quad	.LFB38
	.quad	.LFE38
	.long	.LLST22
	.long	0x41e
	.uleb128 0x13
	.long	.LASF25
	.byte	0x1
	.byte	0x6b
	.long	0x62
	.long	.LLST23
	.uleb128 0x1c
	.long	0xbc
	.quad	.LBB20
	.quad	.LBE20
	.byte	0x1
	.byte	0x6e
	.long	0x3df
	.uleb128 0x1d
	.long	0xce
	.byte	0x0
	.uleb128 0x1c
	.long	0xbc
	.quad	.LBB22
	.quad	.LBE22
	.byte	0x1
	.byte	0x6f
	.long	0x400
	.uleb128 0x1d
	.long	0xce
	.byte	0x0
	.uleb128 0x1e
	.long	0xbc
	.quad	.LBB24
	.quad	.LBE24
	.byte	0x1
	.byte	0x70
	.uleb128 0x1d
	.long	0xce
	.byte	0x0
	.byte	0x0
	.uleb128 0x1f
	.byte	0x1
	.long	.LASF28
	.byte	0x1
	.byte	0x83
	.byte	0x1
	.long	0x62
	.quad	.LFB40
	.quad	.LFE40
	.long	.LLST24
	.long	0x462
	.uleb128 0x17
	.long	.LASF29
	.byte	0x1
	.byte	0x82
	.long	0x62
	.long	.LLST25
	.uleb128 0x17
	.long	.LASF30
	.byte	0x1
	.byte	0x82
	.long	0x462
	.long	.LLST26
	.byte	0x0
	.uleb128 0x6
	.byte	0x8
	.long	0x73
	.uleb128 0x20
	.long	0x79
	.long	0x47b
	.uleb128 0x21
	.long	0x70
	.long	0xfffff
	.byte	0x0
	.uleb128 0x22
	.long	.LASF31
	.byte	0x1
	.byte	0x13
	.long	0x468
	.byte	0x9
	.byte	0x3
	.quad	data
	.uleb128 0x22
	.long	.LASF32
	.byte	0x1
	.byte	0x15
	.long	0x2f9
	.byte	0x9
	.byte	0x3
	.quad	clock_rate
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
	.uleb128 0x16
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
	.uleb128 0x3
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
	.uleb128 0x4
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
	.uleb128 0x5
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
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
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
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
	.uleb128 0xc
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
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
	.uleb128 0x3
	.uleb128 0xe
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
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
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
	.uleb128 0x12
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
	.uleb128 0x13
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
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
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
	.uleb128 0x15
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
	.uleb128 0x16
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
	.uleb128 0x17
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
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
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
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
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
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
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
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
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
	.uleb128 0x1c
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
	.uleb128 0x1d
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
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
	.uleb128 0x1f
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
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x20
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
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
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x6c
	.value	0x2
	.long	.Ldebug_info0
	.long	0x4a6
	.long	0x161
	.string	"strlen"
	.long	0x1a2
	.string	"lower2"
	.long	0x1e6
	.string	"lower1"
	.long	0x220
	.string	"test_lower1"
	.long	0x260
	.string	"test_lower2"
	.long	0x300
	.string	"expo"
	.long	0x38f
	.string	"linear"
	.long	0x41e
	.string	"main"
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
.LASF11:
	.string	"test_funct"
.LASF36:
	.string	"__fmt"
.LASF32:
	.string	"clock_rate"
.LASF38:
	.string	"run_test"
.LASF16:
	.string	"lower1"
.LASF15:
	.string	"lower2"
.LASF37:
	.string	"init"
.LASF12:
	.string	"set_len"
.LASF27:
	.string	"strlen"
.LASF31:
	.string	"data"
.LASF26:
	.string	"linear"
.LASF25:
	.string	"size"
.LASF18:
	.string	"lenp"
.LASF13:
	.string	"unset_len"
.LASF24:
	.string	"expo"
.LASF0:
	.string	"long unsigned int"
.LASF2:
	.string	"short unsigned int"
.LASF10:
	.string	"size_t"
.LASF1:
	.string	"unsigned char"
.LASF22:
	.string	"double"
.LASF28:
	.string	"main"
.LASF17:
	.string	"test_lower1"
.LASF19:
	.string	"test_lower2"
.LASF3:
	.string	"unsigned int"
.LASF8:
	.string	"long long unsigned int"
.LASF35:
	.string	"/home/rbryant/ics2/code/opt"
.LASF21:
	.string	"time"
.LASF29:
	.string	"argc"
.LASF9:
	.string	"long long int"
.LASF7:
	.string	"char"
.LASF34:
	.string	"lower.c"
.LASF23:
	.string	"printf"
.LASF5:
	.string	"short int"
.LASF30:
	.string	"argv"
.LASF6:
	.string	"long int"
.LASF4:
	.string	"signed char"
.LASF14:
	.string	"length"
.LASF20:
	.string	"cycs"
.LASF33:
	.string	"GNU C 4.3.2"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
