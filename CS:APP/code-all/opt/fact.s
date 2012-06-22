	.file	"fact.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
	.p2align 4,,15
.globl rfact
	.type	rfact, @function
rfact:
.LFB14:
	.file 1 "fact.c"
	.loc 1 8 0
.LVL0:
	.loc 1 9 0
	cmpl	$1, %edi
	.loc 1 8 0
	pushq	%rbx
.LCFI0:
	.loc 1 9 0
	movl	$1, %eax
	.loc 1 8 0
	movl	%edi, %ebx
	.loc 1 9 0
	jle	.L3
.LVL1:
	.loc 1 11 0
	leal	-1(%rbx), %edi
	call	rfact
	imull	%ebx, %eax
.LVL2:
.L3:
	.loc 1 12 0
	popq	%rbx
.LVL3:
	ret
.LFE14:
	.size	rfact, .-rfact
	.p2align 4,,15
.globl fact
	.type	fact, @function
fact:
.LFB15:
	.loc 1 16 0
.LVL4:
	.loc 1 20 0
	cmpl	$1, %edi
	movl	$2, %edx
.LVL5:
	movl	$1, %eax
.LVL6:
	jle	.L8
	.p2align 4,,10
	.p2align 3
.L11:
	.loc 1 21 0
	imull	%edx, %eax
	.loc 1 20 0
	addl	$1, %edx
	cmpl	%edx, %edi
	jge	.L11
.L8:
.LVL7:
	.loc 1 23 0
	rep
	ret
.LFE15:
	.size	fact, .-fact
	.p2align 4,,15
.globl fact_u2a
	.type	fact_u2a, @function
fact_u2a:
.LFB16:
	.loc 1 28 0
.LVL8:
	.loc 1 32 0
	cmpl	$2, %edi
	movl	$1, %ecx
.LVL9:
	movl	$2, %edx
.LVL10:
	movl	$2, %eax
.LVL11:
	jle	.L24
	.p2align 4,,10
	.p2align 3
.L21:
	.loc 1 33 0
	leal	1(%rdx), %eax
.LVL12:
	imull	%edx, %eax
	.loc 1 32 0
	addl	$2, %edx
.LVL13:
	.loc 1 33 0
	imull	%eax, %ecx
	.loc 1 32 0
	cmpl	%edx, %edi
	jg	.L21
	leal	-3(%rdi), %eax
	shrl	%eax
	leal	4(%rax,%rax), %eax
.LVL14:
	.loc 1 35 0
	cmpl	%eax, %edi
	jl	.L25
	.p2align 4,,10
	.p2align 3
.L20:
	.loc 1 36 0
	imull	%eax, %ecx
	.loc 1 35 0
	addl	$1, %eax
.L24:
	cmpl	%eax, %edi
	jge	.L20
.L25:
	.loc 1 38 0
	movl	%ecx, %eax
.LVL15:
	ret
.LFE16:
	.size	fact_u2a, .-fact_u2a
	.p2align 4,,15
.globl fact_u2b
	.type	fact_u2b, @function
fact_u2b:
.LFB17:
	.loc 1 42 0
.LVL16:
	.loc 1 46 0
	cmpl	$2, %edi
	movl	$2, %edx
.LVL17:
	movl	$1, %ecx
.LVL18:
	movl	$2, %eax
.LVL19:
	jle	.L28
	.p2align 4,,10
	.p2align 3
.L34:
	.loc 1 48 0
	leal	1(%rdx), %eax
.LVL20:
	imull	%edx, %eax
	.loc 1 46 0
	addl	$2, %edx
.LVL21:
	.loc 1 48 0
	imull	%eax, %ecx
	.loc 1 46 0
	cmpl	%edx, %edi
	jg	.L34
	leal	-3(%rdi), %eax
	shrl	%eax
	leal	4(%rax,%rax), %eax
.LVL22:
.L28:
	.loc 1 51 0
	cmpl	%edi, %eax
	jg	.L30
	.p2align 4,,10
	.p2align 3
.L33:
	.loc 1 52 0
	imull	%eax, %ecx
	.loc 1 51 0
	addl	$1, %eax
	cmpl	%eax, %edi
	jge	.L33
.L30:
	.loc 1 54 0
	movl	%ecx, %eax
.LVL23:
	ret
.LFE17:
	.size	fact_u2b, .-fact_u2b
	.p2align 4,,15
.globl fact_u2c
	.type	fact_u2c, @function
fact_u2c:
.LFB18:
	.loc 1 57 0
.LVL24:
	.loc 1 62 0
	cmpl	$2, %edi
	jle	.L48
	movl	$2, %edx
.LVL25:
	movl	$1, %ecx
.LVL26:
	movl	$1, %esi
.LVL27:
	.p2align 4,,10
	.p2align 3
.L40:
	.loc 1 64 0
	leal	1(%rdx), %eax
	.loc 1 63 0
	imull	%edx, %ecx
	.loc 1 62 0
	addl	$2, %edx
	.loc 1 64 0
	imull	%eax, %esi
	.loc 1 62 0
	cmpl	%edx, %edi
	jg	.L40
	leal	-3(%rdi), %eax
	shrl	%eax
	leal	4(%rax,%rax), %eax
.LVL28:
	jmp	.L47
	.p2align 4,,10
	.p2align 3
.L44:
	.loc 1 67 0
	imull	%eax, %ecx
	.loc 1 66 0
	addl	$1, %eax
.L47:
	cmpl	%eax, %edi
	jge	.L44
	imull	%esi, %ecx
	.loc 1 69 0
	movl	%ecx, %eax
.LVL29:
	ret
.LVL30:
.L48:
	.loc 1 62 0
	movl	$1, %esi
.LVL31:
	movl	$2, %eax
.LVL32:
	movl	$1, %ecx
.LVL33:
	jmp	.L47
.LFE18:
	.size	fact_u2c, .-fact_u2c
	.p2align 4,,15
.globl fact_u3a
	.type	fact_u3a, @function
fact_u3a:
.LFB19:
	.loc 1 73 0
.LVL34:
	.loc 1 77 0
	leal	-1(%rdi), %esi
	movl	$2, %edx
.LVL35:
	movl	$1, %ecx
.LVL36:
	movl	$2, %eax
.LVL37:
	cmpl	$2, %esi
	jle	.L51
	.p2align 4,,10
	.p2align 3
.L57:
	.loc 1 78 0
	imull	%edx, %ecx
	leal	1(%rdx), %eax
.LVL38:
	imull	%eax, %ecx
	leal	2(%rdx), %eax
	.loc 1 77 0
	addl	$3, %edx
	.loc 1 78 0
	imull	%eax, %ecx
	.loc 1 77 0
	cmpl	%esi, %edx
	jl	.L57
	leal	-4(%rdi), %edx
.LVL39:
	movl	$-1431655765, %eax
	mull	%edx
	shrl	%edx
	leal	5(%rdx,%rdx,2), %eax
.LVL40:
.L51:
	.loc 1 80 0
	cmpl	%edi, %eax
	jg	.L53
	.p2align 4,,10
	.p2align 3
.L56:
	.loc 1 81 0
	imull	%eax, %ecx
	.loc 1 80 0
	addl	$1, %eax
	cmpl	%eax, %edi
	jge	.L56
.L53:
	.loc 1 83 0
	movl	%ecx, %eax
.LVL41:
	ret
.LFE19:
	.size	fact_u3a, .-fact_u3a
	.p2align 4,,15
.globl fact_u3b
	.type	fact_u3b, @function
fact_u3b:
.LFB20:
	.loc 1 86 0
.LVL42:
	.loc 1 90 0
	leal	-1(%rdi), %esi
	movl	$2, %edx
.LVL43:
	movl	$1, %ecx
.LVL44:
	movl	$2, %eax
.LVL45:
	cmpl	$2, %esi
	jle	.L62
	.p2align 4,,10
	.p2align 3
.L68:
	.loc 1 91 0
	imull	%edx, %ecx
	leal	1(%rdx), %eax
.LVL46:
	imull	%eax, %ecx
	leal	2(%rdx), %eax
	.loc 1 90 0
	addl	$3, %edx
	.loc 1 91 0
	imull	%eax, %ecx
	.loc 1 90 0
	cmpl	%esi, %edx
	jl	.L68
	leal	-4(%rdi), %edx
.LVL47:
	movl	$-1431655765, %eax
	mull	%edx
	shrl	%edx
	leal	5(%rdx,%rdx,2), %eax
.LVL48:
.L62:
	.loc 1 93 0
	cmpl	%edi, %eax
	jg	.L64
	.p2align 4,,10
	.p2align 3
.L67:
	.loc 1 94 0
	imull	%eax, %ecx
	.loc 1 93 0
	addl	$1, %eax
	cmpl	%eax, %edi
	jge	.L67
.L64:
	.loc 1 96 0
	movl	%ecx, %eax
.LVL49:
	ret
.LFE20:
	.size	fact_u3b, .-fact_u3b
	.p2align 4,,15
.globl fact_u3c
	.type	fact_u3c, @function
fact_u3c:
.LFB21:
	.loc 1 99 0
.LVL50:
	.loc 1 105 0
	leal	-1(%rdi), %r9d
	cmpl	$2, %r9d
	jle	.L82
	movl	$2, %edx
.LVL51:
	movl	$1, %ecx
.LVL52:
	movl	$1, %r8d
.LVL53:
	movl	$1, %esi
.LVL54:
	.p2align 4,,10
	.p2align 3
.L74:
	.loc 1 107 0
	leal	1(%rdx), %eax
	.loc 1 106 0
	imull	%edx, %ecx
	.loc 1 107 0
	imull	%eax, %r8d
	.loc 1 108 0
	leal	2(%rdx), %eax
	.loc 1 105 0
	addl	$3, %edx
	.loc 1 108 0
	imull	%eax, %esi
	.loc 1 105 0
	cmpl	%r9d, %edx
	jl	.L74
	leal	-4(%rdi), %edx
.LVL55:
	movl	$-1431655765, %eax
	mull	%edx
	shrl	%edx
	leal	5(%rdx,%rdx,2), %eax
.LVL56:
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L78:
	.loc 1 111 0
	imull	%eax, %ecx
	.loc 1 110 0
	addl	$1, %eax
.L81:
	cmpl	%eax, %edi
	jge	.L78
	imull	%r8d, %esi
	imull	%ecx, %esi
	.loc 1 113 0
	movl	%esi, %eax
.LVL57:
	ret
.LVL58:
.L82:
	.loc 1 105 0
	movl	$1, %r8d
.LVL59:
	movl	$1, %esi
.LVL60:
	movl	$2, %eax
.LVL61:
	movl	$1, %ecx
.LVL62:
	jmp	.L81
.LFE21:
	.size	fact_u3c, .-fact_u3c
	.p2align 4,,15
.globl rfact_a
	.type	rfact_a, @function
rfact_a:
.LFB22:
	.loc 1 117 0
.LVL63:
	.loc 1 118 0
	cmpl	$1, %edi
	.loc 1 117 0
	pushq	%rbx
.LCFI1:
	.loc 1 118 0
	movl	$1, %eax
	.loc 1 117 0
	movl	%edi, %ebx
	.loc 1 118 0
	jle	.L85
.LVL64:
	.loc 1 120 0
	leal	-1(%rbx), %edi
	call	rfact_a
	negl	%eax
	imull	%ebx, %eax
.LVL65:
.L85:
	.loc 1 121 0
	popq	%rbx
.LVL66:
	ret
.LFE22:
	.size	rfact_a, .-rfact_a
	.p2align 4,,15
	.type	test, @function
test:
.LFB23:
	.loc 1 154 0
.LVL67:
	subq	$8, %rsp
.LCFI2:
	.loc 1 155 0
	movslq	idx(%rip),%rax
	call	*funs(,%rax,8)
.LVL68:
	.loc 1 156 0
	addq	$8, %rsp
	ret
.LFE23:
	.size	test, .-test
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"Oops.  %d! yields %d, should be %d\n"
	.text
	.p2align 4,,15
	.type	ftest, @function
ftest:
.LFB24:
	.loc 1 160 0
.LVL69:
	pushq	%r12
.LCFI3:
	movslq	%edi,%rdi
.LVL70:
	movq	funs(,%rdi,8), %r12
	.loc 1 163 0
	xorl	%edi, %edi
.LVL71:
	.loc 1 160 0
	pushq	%rbp
.LCFI4:
	.loc 1 163 0
	xorl	%ebp, %ebp
.LVL72:
	.loc 1 160 0
	pushq	%rbx
.LCFI5:
	.loc 1 163 0
	call	*%r12
	xorl	%edi, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$1, %edi
	movl	$1, %ebp
.LVL73:
	call	*%r12
	movl	$1, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$2, %edi
	movl	$2, %ebp
	call	*%r12
	movl	$2, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$3, %edi
	movl	$3, %ebp
	call	*%r12
	movl	$3, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$4, %edi
	movl	$4, %ebp
	call	*%r12
	movl	$4, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$5, %edi
	movl	$5, %ebp
	call	*%r12
	movl	$5, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$6, %edi
	movl	$6, %ebp
	call	*%r12
	movl	$6, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$7, %edi
	movl	$7, %ebp
	call	*%r12
	movl	$7, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$8, %edi
	movl	$8, %ebp
	call	*%r12
	movl	$8, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L91
	movl	$9, %edi
	call	*%r12
	movl	$9, %edi
	movl	%eax, %ebx
	call	rfact
	cmpl	%eax, %ebx
	jne	.L102
	.loc 1 169 0
	popq	%rbx
	popq	%rbp
.LVL74:
	popq	%r12
	ret
.LVL75:
	.p2align 4,,10
	.p2align 3
.L102:
	.loc 1 163 0
	movl	$9, %ebp
	.p2align 4,,10
	.p2align 3
.L91:
	.loc 1 164 0
	movl	%ebp, %edi
	call	rfact
	movl	%eax, %ebx
	movl	%ebp, %edi
	call	*%r12
.LBB8:
.LBB9:
	.file 2 "/usr/include/bits/stdio2.h"
	.loc 2 105 0
	movl	%ebx, %r8d
.LBE9:
.LBE8:
	.loc 1 164 0
	movl	%eax, %ecx
.LBB11:
.LBB10:
	.loc 2 105 0
	movl	%ebp, %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
.LBE10:
.LBE11:
	.loc 1 169 0
	popq	%rbx
	popq	%rbp
.LVL76:
	popq	%r12
	ret
.LFE24:
	.size	ftest, .-ftest
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Description: %s, "
.LC2:
	.string	"CPE = %.2f\n"
	.text
	.p2align 4,,15
	.type	run_test, @function
run_test:
.LFB25:
	.loc 1 172 0
.LVL77:
.LBB12:
.LBB14:
	.loc 2 105 0
	movslq	%edi,%rax
.LVL78:
.LBE14:
.LBE12:
	.loc 1 172 0
	pushq	%rbx
.LCFI6:
.LBB16:
.LBB13:
	.loc 2 105 0
	movl	$.LC1, %esi
	movq	test_descr(,%rax,8), %rdx
.LBE13:
.LBE16:
	.loc 1 172 0
	movl	%edi, %ebx
.LBB17:
.LBB15:
	.loc 2 105 0
	xorl	%eax, %eax
.LVL79:
	movl	$1, %edi
	call	__printf_chk
.LBE15:
.LBE17:
	.loc 1 177 0
	movl	$1024, %esi
	movl	$test, %edi
	.loc 1 176 0
	movl	%ebx, idx(%rip)
	.loc 1 177 0
	call	find_cpe
.LBB18:
.LBB19:
	.loc 2 105 0
	movl	$.LC2, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE19:
.LBE18:
	.loc 1 180 0
	movl	%ebx, %edi
	call	ftest
	.loc 1 181 0
	popq	%rbx
.LVL80:
	ret
.LFE25:
	.size	run_test, .-run_test
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB26:
	.loc 1 184 0
	subq	$8, %rsp
.LCFI7:
	.loc 1 187 0
	xorl	%edi, %edi
	call	run_test
	movl	$1, %edi
	call	run_test
	movl	$2, %edi
	call	run_test
	movl	$3, %edi
	call	run_test
	movl	$4, %edi
	call	run_test
	movl	$5, %edi
	call	run_test
	movl	$6, %edi
	call	run_test
	movl	$7, %edi
	call	run_test
	movl	$8, %edi
	call	run_test
	.loc 1 189 0
	xorl	%eax, %eax
	addq	$8, %rsp
	ret
.LFE26:
	.size	main, .-main
.globl idx
	.bss
	.align 4
	.type	idx, @object
	.size	idx, 4
idx:
	.zero	4
	.section	.rodata.str1.1
.LC3:
	.string	"Recursive"
.LC4:
	.string	"Iterative"
.LC5:
	.string	"Unroll 2X Assoc 1"
.LC6:
	.string	"Unroll 2X Assoc 2"
.LC7:
	.string	"Unroll 2X Split"
.LC8:
	.string	"Unroll 3X Assoc 1"
.LC9:
	.string	"Unroll 3X Assoc 2"
.LC10:
	.string	"Unroll 3X Split"
.LC11:
	.string	"Expansion of recursion"
	.section	.rodata
	.align 32
	.type	test_descr, @object
	.size	test_descr, 72
test_descr:
	.quad	.LC3
	.quad	.LC4
	.quad	.LC5
	.quad	.LC6
	.quad	.LC7
	.quad	.LC8
	.quad	.LC9
	.quad	.LC10
	.quad	.LC11
	.align 32
	.type	funs, @object
	.size	funs, 72
funs:
	.quad	rfact
	.quad	fact
	.quad	fact_u2a
	.quad	fact_u2b
	.quad	fact_u2c
	.quad	fact_u3a
	.quad	fact_u3b
	.quad	fact_u3c
	.quad	rfact_a
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
	.quad	.LFB14
	.quad	.LFE14-.LFB14
	.byte	0x4
	.long	.LCFI0-.LFB14
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB15
	.quad	.LFE15-.LFB15
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB16
	.quad	.LFE16-.LFB16
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB17
	.quad	.LFE17-.LFB17
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB18
	.quad	.LFE18-.LFB18
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB19
	.quad	.LFE19-.LFB19
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB20
	.quad	.LFE20-.LFB20
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB21
	.quad	.LFE21-.LFB21
	.align 8
.LEFDE14:
.LSFDE16:
	.long	.LEFDE16-.LASFDE16
.LASFDE16:
	.long	.Lframe0
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.byte	0x4
	.long	.LCFI1-.LFB22
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE16:
.LSFDE18:
	.long	.LEFDE18-.LASFDE18
.LASFDE18:
	.long	.Lframe0
	.quad	.LFB23
	.quad	.LFE23-.LFB23
	.byte	0x4
	.long	.LCFI2-.LFB23
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE18:
.LSFDE20:
	.long	.LEFDE20-.LASFDE20
.LASFDE20:
	.long	.Lframe0
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.byte	0x4
	.long	.LCFI3-.LFB24
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE20:
.LSFDE22:
	.long	.LEFDE22-.LASFDE22
.LASFDE22:
	.long	.Lframe0
	.quad	.LFB25
	.quad	.LFE25-.LFB25
	.byte	0x4
	.long	.LCFI6-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE22:
.LSFDE24:
	.long	.LEFDE24-.LASFDE24
.LASFDE24:
	.long	.Lframe0
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.byte	0x4
	.long	.LCFI7-.LFB26
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE24:
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
	.long	.LFB14
	.long	.LFE14-.LFB14
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB14
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB15
	.long	.LFE15-.LFB15
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB16
	.long	.LFE16-.LFB16
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB17
	.long	.LFE17-.LFB17
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB18
	.long	.LFE18-.LFB18
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB22
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB23
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB24
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI6-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB26
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE25:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.quad	.LFB14-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LFE14-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST1:
	.quad	.LVL0-.Ltext0
	.quad	.LVL1-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL1-.Ltext0
	.quad	.LVL3-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL3-.Ltext0
	.quad	.LFE14-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST3:
	.quad	.LVL6-.Ltext0
	.quad	.LVL7-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST5:
	.quad	.LVL10-.Ltext0
	.quad	.LVL11-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL11-.Ltext0
	.quad	.LVL12-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL12-.Ltext0
	.quad	.LVL14-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL14-.Ltext0
	.quad	.LVL15-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST7:
	.quad	.LVL17-.Ltext0
	.quad	.LVL19-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL19-.Ltext0
	.quad	.LVL20-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL20-.Ltext0
	.quad	.LVL22-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL22-.Ltext0
	.quad	.LVL23-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST9:
	.quad	.LVL25-.Ltext0
	.quad	.LVL28-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL28-.Ltext0
	.quad	.LVL29-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL32-.Ltext0
	.quad	.LFE18-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST10:
	.quad	.LVL26-.Ltext0
	.quad	.LVL30-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL33-.Ltext0
	.quad	.LFE18-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST11:
	.quad	.LVL27-.Ltext0
	.quad	.LVL30-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL31-.Ltext0
	.quad	.LFE18-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
.LLST13:
	.quad	.LVL35-.Ltext0
	.quad	.LVL37-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL37-.Ltext0
	.quad	.LVL38-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL38-.Ltext0
	.quad	.LVL39-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL40-.Ltext0
	.quad	.LVL41-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST15:
	.quad	.LVL43-.Ltext0
	.quad	.LVL45-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL45-.Ltext0
	.quad	.LVL46-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL46-.Ltext0
	.quad	.LVL47-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL48-.Ltext0
	.quad	.LVL49-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST17:
	.quad	.LVL51-.Ltext0
	.quad	.LVL55-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL56-.Ltext0
	.quad	.LVL57-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL61-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL52-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL62-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST19:
	.quad	.LVL53-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL59-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST20:
	.quad	.LVL54-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL60-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
.LLST21:
	.quad	.LFB22-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI1-.Ltext0
	.quad	.LFE22-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST22:
	.quad	.LVL63-.Ltext0
	.quad	.LVL64-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL64-.Ltext0
	.quad	.LVL66-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL66-.Ltext0
	.quad	.LFE22-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST23:
	.quad	.LFB23-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI2-.Ltext0
	.quad	.LFE23-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST24:
	.quad	.LVL67-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST25:
	.quad	.LFB24-.Ltext0
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
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI5-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST26:
	.quad	.LVL69-.Ltext0
	.quad	.LVL71-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST27:
	.quad	.LVL72-.Ltext0
	.quad	.LVL74-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	.LVL75-.Ltext0
	.quad	.LVL76-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	0x0
	.quad	0x0
.LLST28:
	.quad	.LFB25-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI6-.Ltext0
	.quad	.LFE25-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST29:
	.quad	.LVL77-.Ltext0
	.quad	.LVL78-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL78-.Ltext0
	.quad	.LVL79-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL79-.Ltext0
	.quad	.LVL80-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST30:
	.quad	.LFB26-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI7-.Ltext0
	.quad	.LFE26-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
	.section	.debug_info
	.long	0x4de
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF27
	.byte	0x1
	.long	.LASF28
	.long	.LASF29
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
	.uleb128 0x5
	.byte	0x8
	.long	0x6e
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x5
	.byte	0x8
	.long	0x7b
	.uleb128 0x6
	.long	0x6e
	.uleb128 0x7
	.long	.LASF30
	.byte	0x1
	.byte	0x7d
	.long	0x8b
	.uleb128 0x5
	.byte	0x8
	.long	0x91
	.uleb128 0x8
	.byte	0x1
	.long	0x57
	.long	0xa1
	.uleb128 0x9
	.long	0x57
	.byte	0x0
	.uleb128 0xa
	.byte	0x1
	.long	.LASF31
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.long	0x57
	.byte	0x2
	.long	0xc0
	.uleb128 0xb
	.long	.LASF32
	.byte	0x2
	.byte	0x67
	.long	0x75
	.uleb128 0xc
	.byte	0x0
	.uleb128 0xd
	.byte	0x1
	.long	.LASF8
	.byte	0x1
	.byte	0x8
	.byte	0x1
	.long	0x57
	.quad	.LFB14
	.quad	.LFE14
	.long	.LLST0
	.long	0xf3
	.uleb128 0xe
	.string	"n"
	.byte	0x1
	.byte	0x7
	.long	0x57
	.long	.LLST1
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF9
	.byte	0x1
	.byte	0x10
	.byte	0x1
	.long	0x57
	.quad	.LFB15
	.quad	.LFE15
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x13d
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0xf
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x11
	.string	"i"
	.byte	0x1
	.byte	0x11
	.long	0x57
	.byte	0x1
	.byte	0x51
	.uleb128 0x12
	.long	.LASF10
	.byte	0x1
	.byte	0x12
	.long	0x57
	.long	.LLST3
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF11
	.byte	0x1
	.byte	0x1c
	.byte	0x1
	.long	0x57
	.quad	.LFB16
	.quad	.LFE16
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x187
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x1b
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x1d
	.long	0x57
	.long	.LLST5
	.uleb128 0x14
	.long	.LASF10
	.byte	0x1
	.byte	0x1e
	.long	0x57
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF12
	.byte	0x1
	.byte	0x2a
	.byte	0x1
	.long	0x57
	.quad	.LFB17
	.quad	.LFE17
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x1d1
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x29
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x2b
	.long	0x57
	.long	.LLST7
	.uleb128 0x14
	.long	.LASF10
	.byte	0x1
	.byte	0x2c
	.long	0x57
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF13
	.byte	0x1
	.byte	0x39
	.byte	0x1
	.long	0x57
	.quad	.LFB18
	.quad	.LFE18
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x22c
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x38
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x3a
	.long	0x57
	.long	.LLST9
	.uleb128 0x12
	.long	.LASF14
	.byte	0x1
	.byte	0x3b
	.long	0x57
	.long	.LLST10
	.uleb128 0x12
	.long	.LASF15
	.byte	0x1
	.byte	0x3c
	.long	0x57
	.long	.LLST11
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF16
	.byte	0x1
	.byte	0x49
	.byte	0x1
	.long	0x57
	.quad	.LFB19
	.quad	.LFE19
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x276
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x48
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x4a
	.long	0x57
	.long	.LLST13
	.uleb128 0x14
	.long	.LASF10
	.byte	0x1
	.byte	0x4b
	.long	0x57
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF17
	.byte	0x1
	.byte	0x56
	.byte	0x1
	.long	0x57
	.quad	.LFB20
	.quad	.LFE20
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x2c0
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x55
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x57
	.long	0x57
	.long	.LLST15
	.uleb128 0x14
	.long	.LASF10
	.byte	0x1
	.byte	0x58
	.long	0x57
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.long	.LASF18
	.byte	0x1
	.byte	0x63
	.byte	0x1
	.long	0x57
	.quad	.LFB21
	.quad	.LFE21
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x32a
	.uleb128 0x10
	.string	"n"
	.byte	0x1
	.byte	0x62
	.long	0x57
	.byte	0x1
	.byte	0x55
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x64
	.long	0x57
	.long	.LLST17
	.uleb128 0x12
	.long	.LASF14
	.byte	0x1
	.byte	0x65
	.long	0x57
	.long	.LLST18
	.uleb128 0x12
	.long	.LASF15
	.byte	0x1
	.byte	0x66
	.long	0x57
	.long	.LLST19
	.uleb128 0x12
	.long	.LASF19
	.byte	0x1
	.byte	0x67
	.long	0x57
	.long	.LLST20
	.byte	0x0
	.uleb128 0xd
	.byte	0x1
	.long	.LASF20
	.byte	0x1
	.byte	0x75
	.byte	0x1
	.long	0x57
	.quad	.LFB22
	.quad	.LFE22
	.long	.LLST21
	.long	0x35d
	.uleb128 0xe
	.string	"n"
	.byte	0x1
	.byte	0x74
	.long	0x57
	.long	.LLST22
	.byte	0x0
	.uleb128 0x15
	.long	.LASF21
	.byte	0x1
	.byte	0x9a
	.byte	0x1
	.quad	.LFB23
	.quad	.LFE23
	.long	.LLST23
	.long	0x38b
	.uleb128 0xe
	.string	"n"
	.byte	0x1
	.byte	0x99
	.long	0x57
	.long	.LLST24
	.byte	0x0
	.uleb128 0x15
	.long	.LASF22
	.byte	0x1
	.byte	0xa0
	.byte	0x1
	.quad	.LFB24
	.quad	.LFE24
	.long	.LLST25
	.long	0x3d7
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0x9f
	.long	0x57
	.long	.LLST26
	.uleb128 0x13
	.string	"n"
	.byte	0x1
	.byte	0xa1
	.long	0x57
	.long	.LLST27
	.uleb128 0x16
	.long	0xa1
	.long	.Ldebug_ranges0+0x0
	.byte	0x1
	.byte	0xa4
	.uleb128 0x17
	.long	0xb3
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.long	.LASF23
	.byte	0x1
	.byte	0xac
	.byte	0x1
	.quad	.LFB25
	.quad	.LFE25
	.long	.LLST28
	.long	0x442
	.uleb128 0xe
	.string	"i"
	.byte	0x1
	.byte	0xab
	.long	0x57
	.long	.LLST29
	.uleb128 0x18
	.string	"cpe"
	.byte	0x1
	.byte	0xad
	.long	0x442
	.uleb128 0x19
	.long	0xa1
	.long	.Ldebug_ranges0+0x30
	.byte	0x1
	.byte	0xae
	.long	0x424
	.uleb128 0x17
	.long	0xb3
	.byte	0x0
	.uleb128 0x1a
	.long	0xa1
	.quad	.LBB18
	.quad	.LBE18
	.byte	0x1
	.byte	0xb3
	.uleb128 0x17
	.long	0xb3
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF24
	.uleb128 0x1b
	.byte	0x1
	.long	.LASF33
	.byte	0x1
	.byte	0xb8
	.long	0x57
	.quad	.LFB26
	.quad	.LFE26
	.long	.LLST30
	.long	0x477
	.uleb128 0x18
	.string	"i"
	.byte	0x1
	.byte	0xb9
	.long	0x57
	.byte	0x0
	.uleb128 0x1c
	.long	0x80
	.long	0x487
	.uleb128 0x1d
	.long	0x65
	.byte	0x8
	.byte	0x0
	.uleb128 0x14
	.long	.LASF25
	.byte	0x1
	.byte	0x81
	.long	0x49c
	.byte	0x9
	.byte	0x3
	.quad	funs
	.uleb128 0x6
	.long	0x477
	.uleb128 0x1c
	.long	0x68
	.long	0x4b1
	.uleb128 0x1d
	.long	0x65
	.byte	0x8
	.byte	0x0
	.uleb128 0x14
	.long	.LASF26
	.byte	0x1
	.byte	0x89
	.long	0x4c6
	.byte	0x9
	.byte	0x3
	.quad	test_descr
	.uleb128 0x6
	.long	0x4a1
	.uleb128 0x1e
	.string	"idx"
	.byte	0x1
	.byte	0x97
	.long	0x57
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	idx
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
	.uleb128 0x8
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
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
	.uleb128 0xe
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
	.uleb128 0xf
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
	.uleb128 0x12
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
	.uleb128 0x13
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
	.uleb128 0x14
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
	.uleb128 0x15
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
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
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
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
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
	.uleb128 0x1c
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
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
	.long	0x8c
	.value	0x2
	.long	.Ldebug_info0
	.long	0x4e2
	.long	0xc0
	.string	"rfact"
	.long	0xf3
	.string	"fact"
	.long	0x13d
	.string	"fact_u2a"
	.long	0x187
	.string	"fact_u2b"
	.long	0x1d1
	.string	"fact_u2c"
	.long	0x22c
	.string	"fact_u3a"
	.long	0x276
	.string	"fact_u3b"
	.long	0x2c0
	.string	"fact_u3c"
	.long	0x32a
	.string	"rfact_a"
	.long	0x449
	.string	"main"
	.long	0x4cb
	.string	"idx"
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
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.quad	.LBB8-.Ltext0
	.quad	.LBE8-.Ltext0
	.quad	.LBB11-.Ltext0
	.quad	.LBE11-.Ltext0
	.quad	0x0
	.quad	0x0
	.quad	.LBB12-.Ltext0
	.quad	.LBE12-.Ltext0
	.quad	.LBB17-.Ltext0
	.quad	.LBE17-.Ltext0
	.quad	.LBB16-.Ltext0
	.quad	.LBE16-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF29:
	.string	"/home/rbryant/ics2/code/opt"
.LASF10:
	.string	"result"
.LASF27:
	.string	"GNU C 4.3.2"
.LASF28:
	.string	"fact.c"
.LASF19:
	.string	"result2"
.LASF5:
	.string	"short int"
.LASF33:
	.string	"main"
.LASF11:
	.string	"fact_u2a"
.LASF12:
	.string	"fact_u2b"
.LASF13:
	.string	"fact_u2c"
.LASF22:
	.string	"ftest"
.LASF6:
	.string	"long int"
.LASF31:
	.string	"printf"
.LASF25:
	.string	"funs"
.LASF16:
	.string	"fact_u3a"
.LASF17:
	.string	"fact_u3b"
.LASF18:
	.string	"fact_u3c"
.LASF15:
	.string	"result1"
.LASF1:
	.string	"unsigned char"
.LASF4:
	.string	"signed char"
.LASF3:
	.string	"unsigned int"
.LASF26:
	.string	"test_descr"
.LASF2:
	.string	"short unsigned int"
.LASF14:
	.string	"result0"
.LASF7:
	.string	"char"
.LASF20:
	.string	"rfact_a"
.LASF21:
	.string	"test"
.LASF32:
	.string	"__fmt"
.LASF0:
	.string	"long unsigned int"
.LASF9:
	.string	"fact"
.LASF30:
	.string	"fact_fun"
.LASF8:
	.string	"rfact"
.LASF24:
	.string	"double"
.LASF23:
	.string	"run_test"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
