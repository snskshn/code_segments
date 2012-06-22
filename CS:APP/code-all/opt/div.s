	.file	"div.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
.globl itest
	.type	itest, @function
itest:
.LFB20:
	.file 1 "div.c"
	.loc 1 52 0
.LVL0:
	.loc 1 56 0
	mov	(%rdi), %eax
	salq	$11, %rax
	leaq	ival(%rax), %r9
.LVL1:
	.loc 1 57 0
	mov	4(%rdi), %eax
	salq	$11, %rax
	leaq	ival(%rax), %r8
.LVL2:
	.loc 1 61 0
	movl	$0, %ecx
	.loc 1 60 0
	cmpl	$0, 8(%rdi)
	je	.L9
	.loc 1 62 0
	movl	$1, %edi
.LVL3:
.L5:
	movl	(%r8,%rcx), %r10d
	movl	%edi, %esi
	testl	%r10d, %r10d
	js	.L4
.LVL4:
	movl	(%r9,%rcx), %edx
	movl	%edx, %eax
	sarl	$31, %edx
	idivl	%r10d
	movl	%eax, %esi
.LVL5:
.L4:
	movl	%esi, -4(%rsp)
	addq	$4, %rcx
	.loc 1 61 0
	cmpq	$2048, %rcx
	jne	.L5
.LVL6:
	rep
	ret
.LVL7:
.L9:
	.loc 1 65 0
	movl	(%r9,%rcx), %edx
	movl	%edx, %eax
	sarl	$31, %edx
	idivl	(%r8,%rcx)
	movl	%eax, -4(%rsp)
.LVL8:
	addq	$4, %rcx
	.loc 1 64 0
	cmpq	$2048, %rcx
	jne	.L9
	rep
	ret
.LFE20:
	.size	itest, .-itest
.globl ltest
	.type	ltest, @function
ltest:
.LFB21:
	.loc 1 70 0
.LVL9:
	.loc 1 74 0
	mov	(%rdi), %eax
	salq	$12, %rax
	leaq	lval(%rax), %r9
.LVL10:
	.loc 1 75 0
	mov	4(%rdi), %eax
	salq	$12, %rax
	leaq	lval(%rax), %r8
.LVL11:
	.loc 1 79 0
	movl	$0, %ecx
	.loc 1 78 0
	cmpl	$0, 8(%rdi)
	je	.L20
	.loc 1 80 0
	movl	$1, %edi
.LVL12:
.L16:
	movq	(%r8,%rcx), %r10
	movq	%rdi, %rsi
	testq	%r10, %r10
	js	.L15
.LVL13:
	movq	(%r9,%rcx), %rdx
	movq	%rdx, %rax
	sarq	$63, %rdx
	idivq	%r10
	movq	%rax, %rsi
.LVL14:
.L15:
	movq	%rsi, -8(%rsp)
	addq	$8, %rcx
	.loc 1 79 0
	cmpq	$4096, %rcx
	jne	.L16
.LVL15:
	rep
	ret
.LVL16:
.L20:
	.loc 1 83 0
	movq	(%r9,%rcx), %rdx
	movq	%rdx, %rax
	sarq	$63, %rdx
	idivq	(%r8,%rcx)
	movq	%rax, -8(%rsp)
.LVL17:
	addq	$8, %rcx
	.loc 1 82 0
	cmpq	$4096, %rcx
	jne	.L20
	rep
	ret
.LFE21:
	.size	ltest, .-ltest
.globl ftest
	.type	ftest, @function
ftest:
.LFB22:
	.loc 1 88 0
.LVL18:
	.loc 1 92 0
	mov	(%rdi), %eax
	salq	$11, %rax
	leaq	fval(%rax), %rcx
.LVL19:
	.loc 1 93 0
	mov	4(%rdi), %eax
	salq	$11, %rax
	addq	$fval, %rax
.LVL20:
	.loc 1 97 0
	movl	$0, %edx
	.loc 1 96 0
	cmpl	$0, 8(%rdi)
	je	.L33
	.loc 1 98 0
	xorps	%xmm2, %xmm2
	movl	$0x3f800000, %esi
.LVL21:
.L28:
	movss	(%rax,%rdx), %xmm1
	movl	%esi, -20(%rsp)
	movss	-20(%rsp), %xmm0
.LVL22:
	ucomiss	%xmm1, %xmm2
	ja	.L27
	movss	(%rcx,%rdx), %xmm0
.LVL23:
	divss	%xmm1, %xmm0
.LVL24:
.L27:
	movss	%xmm0, -4(%rsp)
	addq	$4, %rdx
	.loc 1 97 0
	cmpq	$2048, %rdx
	jne	.L28
.LVL25:
	rep
	ret
.LVL26:
.L33:
	.loc 1 101 0
	movss	(%rcx,%rdx), %xmm0
	divss	(%rax,%rdx), %xmm0
.LVL27:
	movss	%xmm0, -4(%rsp)
	addq	$4, %rdx
	.loc 1 100 0
	cmpq	$2048, %rdx
	jne	.L33
.LVL28:
	rep
	ret
.LFE22:
	.size	ftest, .-ftest
.globl dtest
	.type	dtest, @function
dtest:
.LFB23:
	.loc 1 106 0
.LVL29:
	.loc 1 110 0
	mov	(%rdi), %eax
	salq	$12, %rax
	leaq	dval(%rax), %rcx
.LVL30:
	.loc 1 111 0
	mov	4(%rdi), %eax
	salq	$12, %rax
	addq	$dval, %rax
.LVL31:
	.loc 1 115 0
	movl	$0, %edx
	.loc 1 114 0
	cmpl	$0, 8(%rdi)
	je	.L47
	.loc 1 116 0
	xorpd	%xmm2, %xmm2
	movabsq	$4607182418800017408, %rsi
.LVL32:
.L42:
	movsd	(%rax,%rdx), %xmm1
	movq	%rsi, -24(%rsp)
	movsd	-24(%rsp), %xmm0
.LVL33:
	ucomisd	%xmm1, %xmm2
	ja	.L41
	movsd	(%rcx,%rdx), %xmm0
.LVL34:
	divsd	%xmm1, %xmm0
.LVL35:
.L41:
	movsd	%xmm0, -8(%rsp)
	addq	$8, %rdx
	.loc 1 115 0
	cmpq	$4096, %rdx
	jne	.L42
.LVL36:
	rep
	ret
.LVL37:
.L47:
	.loc 1 119 0
	movsd	(%rcx,%rdx), %xmm0
	divsd	(%rax,%rdx), %xmm0
.LVL38:
	movsd	%xmm0, -8(%rsp)
	addq	$8, %rdx
	.loc 1 118 0
	cmpq	$4096, %rdx
	jne	.L47
.LVL39:
	rep
	ret
.LFE23:
	.size	dtest, .-dtest
	.type	init, @function
init:
.LFB19:
	.loc 1 17 0
	pushq	%r15
.LCFI0:
	pushq	%r14
.LCFI1:
	pushq	%r13
.LCFI2:
	pushq	%r12
.LCFI3:
	pushq	%rbp
.LCFI4:
	pushq	%rbx
.LCFI5:
	subq	$8, %rsp
.LCFI6:
	.loc 1 17 0
	movl	$ival+6144, %r14d
	movl	$0, %r13d
.LVL40:
	jmp	.L52
.LVL41:
.L53:
.LBB12:
	.loc 1 23 0
	call	random
.LVL42:
	movl	%eax, %edx
.LVL43:
	.loc 1 22 0
	testl	%eax, %eax
	je	.L53
.LVL44:
	sarl	$31, %eax
.LVL45:
	movl	%eax, %ebp
.LVL46:
	xorl	%edx, %ebp
	subl	%eax, %ebp
	.loc 1 26 0
	cmpl	$1, %r12d
	je	.L56
	cmpl	$1, %r12d
	jg	.L59
	testl	%r12d, %r12d
	je	.L55
	.p2align 4,,7
	.p2align 3
	jmp	.L54
.L59:
	cmpl	$2, %r12d
	.p2align 4,,7
	.p2align 3
	je	.L57
	cmpl	$3, %r12d
	.p2align 4,,7
	.p2align 3
	jne	.L54
	.p2align 4,,7
	.p2align 3
	jmp	.L70
.L55:
	.loc 1 28 0
	movl	%ebp, %eax
	negl	%eax
	movl	%eax, -6144(%r14)
	jmp	.L60
.L56:
	.loc 1 31 0
	movl	%ebp, -4096(%r14)
	jmp	.L60
.L57:
	.loc 1 34 0
	movl	%ebp, %eax
	negl	%eax
	cmpl	$0, 4(%rsp)
	cmove	%eax, %ebp
	movl	%ebp, -2048(%r14)
	jmp	.L60
.L70:
	.loc 1 35 0
	movl	%ebp, %ebx
	.loc 1 37 0
	call	random
	negl	%ebx
	testb	$1, %al
	cmove	%ebx, %ebp
	movl	%ebp, (%r14)
	.loc 1 40 0
	movslq	%r13d,%rdx
.LVL47:
	movslq	%r12d,%rax
.LVL48:
	salq	$9, %rax
.LVL49:
	addq	%rdx, %rax
	movl	ival(,%rax,4), %ecx
	movslq	%ecx,%rdx
.LVL50:
	movq	%rdx, lval(,%rax,8)
	.loc 1 41 0
	cvtsi2ss	%ecx, %xmm0
	movss	%xmm0, fval(,%rax,4)
	.loc 1 42 0
	cvtsi2sd	%ecx, %xmm0
	movsd	%xmm0, dval(,%rax,8)
	jmp	.L63
.LVL51:
.L54:
	.loc 1 40 0
	movslq	%r12d,%rax
.LVL52:
	salq	$9, %rax
.LVL53:
	addq	%r15, %rax
	movl	ival(,%rax,4), %ecx
	movslq	%ecx,%rdx
	movq	%rdx, lval(,%rax,8)
	.loc 1 41 0
	cvtsi2ss	%ecx, %xmm0
	movss	%xmm0, fval(,%rax,4)
	.loc 1 42 0
	cvtsi2sd	%ecx, %xmm0
	movsd	%xmm0, dval(,%rax,8)
.LBE12:
	.loc 1 20 0
	cmpl	$3, %ebx
	jle	.L64
.LVL54:
.L63:
	.loc 1 19 0
	addl	$1, %r13d
.LVL55:
	addq	$4, %r14
	cmpl	$512, %r13d
	jne	.L52
	jmp	.L71
.LVL56:
.L64:
	.loc 1 45 0
	addl	$1, %ebx
.L65:
	leal	-1(%rbx), %r12d
.LVL57:
	jmp	.L53
.LVL58:
.L52:
	movl	$1, %ebx
.LBB13:
	.loc 1 40 0
	movslq	%r13d,%r15
.LVL59:
	.loc 1 34 0
	movl	%r13d, %eax
	andl	$1, %eax
	movl	%eax, 4(%rsp)
	jmp	.L65
.LVL60:
.L60:
	.loc 1 40 0
	movslq	%r12d,%rax
.LVL61:
	salq	$9, %rax
.LVL62:
	addq	%r15, %rax
	movl	ival(,%rax,4), %ecx
	movslq	%ecx,%rdx
	movq	%rdx, lval(,%rax,8)
	.loc 1 41 0
	cvtsi2ss	%ecx, %xmm0
	movss	%xmm0, fval(,%rax,4)
	.loc 1 42 0
	cvtsi2sd	%ecx, %xmm0
	movsd	%xmm0, dval(,%rax,8)
	jmp	.L64
.LVL63:
.L71:
.LBE13:
	.loc 1 45 0
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
.LVL64:
	popq	%r12
	popq	%r13
.LVL65:
	popq	%r14
	popq	%r15
	ret
.LFE19:
	.size	init, .-init
.globl cpt
	.type	cpt, @function
cpt:
.LFB24:
	.loc 1 124 0
.LVL66:
	subq	$24, %rsp
.LCFI7:
	.loc 1 125 0
	movl	%esi, (%rsp)
	movl	%edx, 4(%rsp)
	movl	%ecx, 8(%rsp)
	.loc 1 126 0
	movq	%rsp, %rsi
.LVL67:
	call	fcyc
.LVL68:
	.loc 1 128 0
	mulsd	.LC4(%rip), %xmm0
	addq	$24, %rsp
	ret
.LFE24:
	.size	cpt, .-cpt
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"Function: %s\t  div(+,   -)\tCPD = %.2f\n"
	.align 8
.LC6:
	.string	"Function: %s\t cdiv(+,   -)\tCPD = %.2f\n"
	.align 8
.LC7:
	.string	"Function: %s\t cdiv(-,   +)\tCPD = %.2f\n"
	.align 8
.LC8:
	.string	"Function: %s\t cdiv(+, a+-)\tCPD = %.2f\n"
	.align 8
.LC9:
	.string	"Function: %s\t cdiv(+, r+-)\tCPD = %.2f\n"
	.text
.globl run_tests
	.type	run_tests, @function
run_tests:
.LFB25:
	.loc 1 131 0
.LVL69:
	pushq	%r12
.LCFI8:
	pushq	%rbx
.LCFI9:
	subq	$8, %rsp
.LCFI10:
	movq	%rdi, %rbx
	movq	%rsi, %r12
	.loc 1 132 0
	movl	$0, %ecx
	movl	$0, %edx
	movl	$1, %esi
.LVL70:
	call	cpt
.LVL71:
.LBB14:
.LBB15:
	.file 2 "/usr/include/bits/stdio2.h"
	.loc 2 105 0
	movq	%r12, %rdx
	movl	$.LC5, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE15:
.LBE14:
	.loc 1 134 0
	movl	$1, %ecx
	movl	$0, %edx
	movl	$1, %esi
	movq	%rbx, %rdi
	call	cpt
.LBB16:
.LBB17:
	.loc 2 105 0
	movq	%r12, %rdx
	movl	$.LC6, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE17:
.LBE16:
	.loc 1 136 0
	movl	$1, %ecx
	movl	$1, %edx
	movl	$0, %esi
	movq	%rbx, %rdi
	call	cpt
.LBB18:
.LBB19:
	.loc 2 105 0
	movq	%r12, %rdx
	movl	$.LC7, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE19:
.LBE18:
	.loc 1 138 0
	movl	$1, %ecx
	movl	$2, %edx
	movl	$1, %esi
	movq	%rbx, %rdi
	call	cpt
.LBB20:
.LBB21:
	.loc 2 105 0
	movq	%r12, %rdx
	movl	$.LC8, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE21:
.LBE20:
	.loc 1 140 0
	movl	$1, %ecx
	movl	$3, %edx
	movl	$1, %esi
	movq	%rbx, %rdi
	call	cpt
.LBB22:
.LBB23:
	.loc 2 105 0
	movq	%r12, %rdx
	movl	$.LC9, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE23:
.LBE22:
	.loc 1 142 0
	addq	$8, %rsp
	popq	%rbx
.LVL72:
	popq	%r12
.LVL73:
	ret
.LFE25:
	.size	run_tests, .-run_tests
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC10:
	.string	"int   "
.LC11:
	.string	"long  "
.LC12:
	.string	"float "
.LC13:
	.string	"double"
	.text
.globl main
	.type	main, @function
main:
.LFB26:
	.loc 1 145 0
	subq	$8, %rsp
.LCFI11:
	.loc 1 146 0
	movl	$0, %eax
	call	init
	.loc 1 147 0
	movl	$.LC10, %esi
	movl	$itest, %edi
	call	run_tests
	.loc 1 148 0
	movl	$.LC11, %esi
	movl	$ltest, %edi
	call	run_tests
	.loc 1 149 0
	movl	$.LC12, %esi
	movl	$ftest, %edi
	call	run_tests
	.loc 1 150 0
	movl	$.LC13, %esi
	movl	$dtest, %edi
	call	run_tests
	.loc 1 152 0
	movl	$0, %eax
	addq	$8, %rsp
	ret
.LFE26:
	.size	main, .-main
.globl num_type
	.bss
	.align 4
	.type	num_type, @object
	.size	num_type, 4
num_type:
	.zero	4
.globl den_type
	.align 4
	.type	den_type, @object
	.size	den_type, 4
den_type:
	.zero	4
.globl use_cond
	.align 4
	.type	use_cond, @object
	.size	use_cond, 4
use_cond:
	.zero	4
	.comm	ival,10240,32
	.comm	lval,20480,32
	.comm	fval,10240,32
	.comm	dval,20480,32
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
	.long	0
	.long	1063256064
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
	.quad	.LFB20
	.quad	.LFE20-.LFB20
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB21
	.quad	.LFE21-.LFB21
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB22
	.quad	.LFE22-.LFB22
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB23
	.quad	.LFE23-.LFB23
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB19
	.quad	.LFE19-.LFB19
	.byte	0x4
	.long	.LCFI0-.LFB19
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.byte	0x4
	.long	.LCFI7-.LFB24
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB25
	.quad	.LFE25-.LFB25
	.byte	0x4
	.long	.LCFI8-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.byte	0x4
	.long	.LCFI11-.LFB26
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE14:
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
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB19
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x7
	.byte	0x86
	.uleb128 0x6
	.byte	0x8c
	.uleb128 0x5
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI7-.LFB24
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI11-.LFB26
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE15:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST1:
	.quad	.LVL0-.Ltext0
	.quad	.LVL3-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL7-.Ltext0
	.quad	.LFE20-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST2:
	.quad	.LVL3-.Ltext0
	.quad	.LVL4-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL4-.Ltext0
	.quad	.LVL6-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL6-.Ltext0
	.quad	.LVL7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -4
	.quad	.LVL8-.Ltext0
	.quad	.LFE20-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -4
	.quad	0x0
	.quad	0x0
.LLST4:
	.quad	.LVL9-.Ltext0
	.quad	.LVL12-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL16-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST5:
	.quad	.LVL12-.Ltext0
	.quad	.LVL13-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL13-.Ltext0
	.quad	.LVL15-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL15-.Ltext0
	.quad	.LVL16-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -8
	.quad	.LVL17-.Ltext0
	.quad	.LFE21-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -8
	.quad	0x0
	.quad	0x0
.LLST7:
	.quad	.LVL21-.Ltext0
	.quad	.LVL22-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL22-.Ltext0
	.quad	.LVL23-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL24-.Ltext0
	.quad	.LVL25-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL25-.Ltext0
	.quad	.LVL26-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -4
	.quad	.LVL27-.Ltext0
	.quad	.LVL28-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL28-.Ltext0
	.quad	.LFE22-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -4
	.quad	0x0
	.quad	0x0
.LLST9:
	.quad	.LVL32-.Ltext0
	.quad	.LVL33-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL33-.Ltext0
	.quad	.LVL34-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL35-.Ltext0
	.quad	.LVL36-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL36-.Ltext0
	.quad	.LVL37-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -8
	.quad	.LVL38-.Ltext0
	.quad	.LVL39-.Ltext0
	.value	0x1
	.byte	0x61
	.quad	.LVL39-.Ltext0
	.quad	.LFE23-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 -8
	.quad	0x0
	.quad	0x0
.LLST10:
	.quad	.LFB19-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI1-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI2-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	.LCFI3-.Ltext0
	.quad	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 40
	.quad	.LCFI4-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 48
	.quad	.LCFI5-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 56
	.quad	.LCFI6-.Ltext0
	.quad	.LFE19-.Ltext0
	.value	0x3
	.byte	0x77
	.sleb128 64
	.quad	0x0
	.quad	0x0
.LLST11:
	.quad	.LVL40-.Ltext0
	.quad	.LVL47-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	.LVL47-.Ltext0
	.quad	.LVL50-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL51-.Ltext0
	.quad	.LVL54-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	.LVL54-.Ltext0
	.quad	.LVL55-.Ltext0
	.value	0x1
	.byte	0x5f
	.quad	.LVL55-.Ltext0
	.quad	.LVL56-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	.LVL56-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x5f
	.quad	.LVL58-.Ltext0
	.quad	.LVL59-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	.LVL59-.Ltext0
	.quad	.LVL60-.Ltext0
	.value	0x1
	.byte	0x5f
	.quad	.LVL60-.Ltext0
	.quad	.LVL65-.Ltext0
	.value	0x1
	.byte	0x5d
	.quad	0x0
	.quad	0x0
.LLST12:
	.quad	.LVL41-.Ltext0
	.quad	.LVL48-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL48-.Ltext0
	.quad	.LVL49-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL51-.Ltext0
	.quad	.LVL52-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL52-.Ltext0
	.quad	.LVL53-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL57-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL60-.Ltext0
	.quad	.LVL61-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	.LVL61-.Ltext0
	.quad	.LVL62-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST13:
	.quad	.LVL41-.Ltext0
	.quad	.LVL42-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL42-.Ltext0
	.quad	.LVL43-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	.LVL43-.Ltext0
	.quad	.LVL44-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL44-.Ltext0
	.quad	.LVL45-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL45-.Ltext0
	.quad	.LVL46-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL46-.Ltext0
	.quad	.LVL64-.Ltext0
	.value	0x1
	.byte	0x56
	.quad	0x0
	.quad	0x0
.LLST14:
	.quad	.LFB24-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI7-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST15:
	.quad	.LVL66-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST16:
	.quad	.LVL66-.Ltext0
	.quad	.LVL67-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	0x0
	.quad	0x0
.LLST17:
	.quad	.LVL66-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL66-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST19:
	.quad	.LFB25-.Ltext0
	.quad	.LCFI8-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI8-.Ltext0
	.quad	.LCFI9-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI9-.Ltext0
	.quad	.LCFI10-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI10-.Ltext0
	.quad	.LFE25-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST20:
	.quad	.LVL69-.Ltext0
	.quad	.LVL71-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL71-.Ltext0
	.quad	.LVL72-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST21:
	.quad	.LVL69-.Ltext0
	.quad	.LVL70-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL70-.Ltext0
	.quad	.LVL73-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST22:
	.quad	.LFB26-.Ltext0
	.quad	.LCFI11-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI11-.Ltext0
	.quad	.LFE26-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
	.file 3 "../include/fcyc.h"
	.section	.debug_info
	.long	0x608
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF35
	.byte	0x1
	.long	.LASF36
	.long	.LASF37
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
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF9
	.uleb128 0x7
	.long	0x57
	.uleb128 0x8
	.long	.LASF15
	.byte	0x3
	.byte	0x7
	.long	0x9e
	.uleb128 0x5
	.byte	0x8
	.long	0xa4
	.uleb128 0x9
	.byte	0x1
	.long	0xb0
	.uleb128 0xa
	.long	0xb0
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0x57
	.uleb128 0xb
	.byte	0x4
	.byte	0x1
	.byte	0x8
	.long	0xdd
	.uleb128 0xc
	.long	.LASF10
	.sleb128 0
	.uleb128 0xc
	.long	.LASF11
	.sleb128 1
	.uleb128 0xc
	.long	.LASF12
	.sleb128 2
	.uleb128 0xc
	.long	.LASF13
	.sleb128 3
	.uleb128 0xc
	.long	.LASF14
	.sleb128 4
	.byte	0x0
	.uleb128 0x8
	.long	.LASF16
	.byte	0x1
	.byte	0x8
	.long	0xb6
	.uleb128 0xd
	.byte	0x1
	.long	.LASF38
	.byte	0x2
	.byte	0x68
	.byte	0x1
	.long	0x57
	.byte	0x2
	.long	0x107
	.uleb128 0xe
	.long	.LASF39
	.byte	0x2
	.byte	0x67
	.long	0x75
	.uleb128 0xf
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF21
	.byte	0x1
	.byte	0x34
	.byte	0x1
	.quad	.LFB20
	.quad	.LFE20
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x18a
	.uleb128 0x11
	.long	.LASF23
	.byte	0x1
	.byte	0x33
	.long	0xb0
	.long	.LLST1
	.uleb128 0x12
	.long	.LASF17
	.byte	0x1
	.byte	0x35
	.long	0xdd
	.uleb128 0x12
	.long	.LASF18
	.byte	0x1
	.byte	0x36
	.long	0xdd
	.uleb128 0x12
	.long	.LASF19
	.byte	0x1
	.byte	0x37
	.long	0x57
	.uleb128 0x13
	.string	"num"
	.byte	0x1
	.byte	0x38
	.long	0xb0
	.byte	0x1
	.byte	0x59
	.uleb128 0x13
	.string	"den"
	.byte	0x1
	.byte	0x39
	.long	0xb0
	.byte	0x1
	.byte	0x58
	.uleb128 0x14
	.long	.LASF20
	.byte	0x1
	.byte	0x3a
	.long	0x8e
	.long	.LLST2
	.uleb128 0x15
	.string	"i"
	.byte	0x1
	.byte	0x3b
	.long	0x57
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF22
	.byte	0x1
	.byte	0x46
	.byte	0x1
	.quad	.LFB21
	.quad	.LFE21
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x20d
	.uleb128 0x11
	.long	.LASF23
	.byte	0x1
	.byte	0x45
	.long	0xb0
	.long	.LLST4
	.uleb128 0x12
	.long	.LASF17
	.byte	0x1
	.byte	0x47
	.long	0xdd
	.uleb128 0x12
	.long	.LASF18
	.byte	0x1
	.byte	0x48
	.long	0xdd
	.uleb128 0x12
	.long	.LASF19
	.byte	0x1
	.byte	0x49
	.long	0x57
	.uleb128 0x13
	.string	"num"
	.byte	0x1
	.byte	0x4a
	.long	0x20d
	.byte	0x1
	.byte	0x59
	.uleb128 0x13
	.string	"den"
	.byte	0x1
	.byte	0x4b
	.long	0x20d
	.byte	0x1
	.byte	0x58
	.uleb128 0x14
	.long	.LASF20
	.byte	0x1
	.byte	0x4c
	.long	0x213
	.long	.LLST5
	.uleb128 0x15
	.string	"i"
	.byte	0x1
	.byte	0x4d
	.long	0x57
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0x5e
	.uleb128 0x7
	.long	0x5e
	.uleb128 0x10
	.byte	0x1
	.long	.LASF24
	.byte	0x1
	.byte	0x58
	.byte	0x1
	.quad	.LFB22
	.quad	.LFE22
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x299
	.uleb128 0x16
	.long	.LASF23
	.byte	0x1
	.byte	0x57
	.long	0xb0
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.long	.LASF17
	.byte	0x1
	.byte	0x59
	.long	0xdd
	.uleb128 0x12
	.long	.LASF18
	.byte	0x1
	.byte	0x5a
	.long	0xdd
	.uleb128 0x12
	.long	.LASF19
	.byte	0x1
	.byte	0x5b
	.long	0x57
	.uleb128 0x13
	.string	"num"
	.byte	0x1
	.byte	0x5c
	.long	0x299
	.byte	0x1
	.byte	0x52
	.uleb128 0x13
	.string	"den"
	.byte	0x1
	.byte	0x5d
	.long	0x299
	.byte	0x1
	.byte	0x50
	.uleb128 0x14
	.long	.LASF20
	.byte	0x1
	.byte	0x5e
	.long	0x2a6
	.long	.LLST7
	.uleb128 0x15
	.string	"i"
	.byte	0x1
	.byte	0x5f
	.long	0x57
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0x29f
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.long	.LASF25
	.uleb128 0x7
	.long	0x29f
	.uleb128 0x10
	.byte	0x1
	.long	.LASF26
	.byte	0x1
	.byte	0x6a
	.byte	0x1
	.quad	.LFB23
	.quad	.LFE23
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x32c
	.uleb128 0x16
	.long	.LASF23
	.byte	0x1
	.byte	0x69
	.long	0xb0
	.byte	0x1
	.byte	0x55
	.uleb128 0x12
	.long	.LASF17
	.byte	0x1
	.byte	0x6b
	.long	0xdd
	.uleb128 0x12
	.long	.LASF18
	.byte	0x1
	.byte	0x6c
	.long	0xdd
	.uleb128 0x12
	.long	.LASF19
	.byte	0x1
	.byte	0x6d
	.long	0x57
	.uleb128 0x13
	.string	"num"
	.byte	0x1
	.byte	0x6e
	.long	0x32c
	.byte	0x1
	.byte	0x52
	.uleb128 0x13
	.string	"den"
	.byte	0x1
	.byte	0x6f
	.long	0x32c
	.byte	0x1
	.byte	0x50
	.uleb128 0x14
	.long	.LASF20
	.byte	0x1
	.byte	0x70
	.long	0x339
	.long	.LLST9
	.uleb128 0x15
	.string	"i"
	.byte	0x1
	.byte	0x71
	.long	0x57
	.byte	0x0
	.uleb128 0x5
	.byte	0x8
	.long	0x332
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.long	.LASF27
	.uleb128 0x7
	.long	0x332
	.uleb128 0x17
	.long	.LASF40
	.byte	0x1
	.byte	0x11
	.quad	.LFB19
	.quad	.LFE19
	.long	.LLST10
	.long	0x38d
	.uleb128 0x18
	.string	"i"
	.byte	0x1
	.byte	0x12
	.long	0x57
	.long	.LLST11
	.uleb128 0x18
	.string	"j"
	.byte	0x1
	.byte	0x12
	.long	0x57
	.long	.LLST12
	.uleb128 0x19
	.long	.Ldebug_ranges0+0x0
	.uleb128 0x18
	.string	"val"
	.byte	0x1
	.byte	0x15
	.long	0x57
	.long	.LLST13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
	.byte	0x1
	.string	"cpt"
	.byte	0x1
	.byte	0x7c
	.byte	0x1
	.long	0x332
	.quad	.LFB24
	.quad	.LFE24
	.long	.LLST14
	.long	0x405
	.uleb128 0x1b
	.string	"tf"
	.byte	0x1
	.byte	0x7b
	.long	0x93
	.long	.LLST15
	.uleb128 0x11
	.long	.LASF17
	.byte	0x1
	.byte	0x7b
	.long	0xdd
	.long	.LLST16
	.uleb128 0x11
	.long	.LASF18
	.byte	0x1
	.byte	0x7b
	.long	0xdd
	.long	.LLST17
	.uleb128 0x11
	.long	.LASF19
	.byte	0x1
	.byte	0x7b
	.long	0x57
	.long	.LLST18
	.uleb128 0x1c
	.long	.LASF28
	.byte	0x1
	.byte	0x7d
	.long	0x405
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x15
	.string	"t"
	.byte	0x1
	.byte	0x7e
	.long	0x332
	.byte	0x0
	.uleb128 0x1d
	.long	0x57
	.long	0x415
	.uleb128 0x1e
	.long	0x65
	.byte	0x2
	.byte	0x0
	.uleb128 0x1f
	.byte	0x1
	.long	.LASF29
	.byte	0x1
	.byte	0x83
	.byte	0x1
	.quad	.LFB25
	.quad	.LFE25
	.long	.LLST19
	.long	0x4f5
	.uleb128 0x1b
	.string	"tf"
	.byte	0x1
	.byte	0x82
	.long	0x93
	.long	.LLST20
	.uleb128 0x11
	.long	.LASF30
	.byte	0x1
	.byte	0x82
	.long	0x68
	.long	.LLST21
	.uleb128 0x20
	.long	0xe8
	.quad	.LBB14
	.quad	.LBE14
	.byte	0x1
	.byte	0x84
	.long	0x474
	.uleb128 0x21
	.long	0xfa
	.byte	0x0
	.uleb128 0x20
	.long	0xe8
	.quad	.LBB16
	.quad	.LBE16
	.byte	0x1
	.byte	0x86
	.long	0x495
	.uleb128 0x21
	.long	0xfa
	.byte	0x0
	.uleb128 0x20
	.long	0xe8
	.quad	.LBB18
	.quad	.LBE18
	.byte	0x1
	.byte	0x88
	.long	0x4b6
	.uleb128 0x21
	.long	0xfa
	.byte	0x0
	.uleb128 0x20
	.long	0xe8
	.quad	.LBB20
	.quad	.LBE20
	.byte	0x1
	.byte	0x8a
	.long	0x4d7
	.uleb128 0x21
	.long	0xfa
	.byte	0x0
	.uleb128 0x22
	.long	0xe8
	.quad	.LBB22
	.quad	.LBE22
	.byte	0x1
	.byte	0x8c
	.uleb128 0x21
	.long	0xfa
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
	.byte	0x1
	.long	.LASF41
	.byte	0x1
	.byte	0x91
	.long	0x57
	.quad	.LFB26
	.quad	.LFE26
	.long	.LLST22
	.uleb128 0x1d
	.long	0x57
	.long	0x52c
	.uleb128 0x1e
	.long	0x65
	.byte	0x4
	.uleb128 0x24
	.long	0x65
	.value	0x1ff
	.byte	0x0
	.uleb128 0x25
	.long	.LASF31
	.byte	0x1
	.byte	0xb
	.long	0x515
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	ival
	.uleb128 0x1d
	.long	0x5e
	.long	0x559
	.uleb128 0x1e
	.long	0x65
	.byte	0x4
	.uleb128 0x24
	.long	0x65
	.value	0x1ff
	.byte	0x0
	.uleb128 0x25
	.long	.LASF32
	.byte	0x1
	.byte	0xc
	.long	0x542
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	lval
	.uleb128 0x1d
	.long	0x29f
	.long	0x586
	.uleb128 0x1e
	.long	0x65
	.byte	0x4
	.uleb128 0x24
	.long	0x65
	.value	0x1ff
	.byte	0x0
	.uleb128 0x25
	.long	.LASF33
	.byte	0x1
	.byte	0xd
	.long	0x56f
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	fval
	.uleb128 0x1d
	.long	0x332
	.long	0x5b3
	.uleb128 0x1e
	.long	0x65
	.byte	0x4
	.uleb128 0x24
	.long	0x65
	.value	0x1ff
	.byte	0x0
	.uleb128 0x25
	.long	.LASF34
	.byte	0x1
	.byte	0xe
	.long	0x59c
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	dval
	.uleb128 0x25
	.long	.LASF17
	.byte	0x1
	.byte	0x2f
	.long	0xdd
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	num_type
	.uleb128 0x25
	.long	.LASF18
	.byte	0x1
	.byte	0x30
	.long	0xdd
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	den_type
	.uleb128 0x25
	.long	.LASF19
	.byte	0x1
	.byte	0x31
	.long	0x57
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	use_cond
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
	.uleb128 0x35
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
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
	.uleb128 0x9
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x4
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x28
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xd
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
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
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
	.uleb128 0xf
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
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
	.uleb128 0x11
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
	.uleb128 0xa
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
	.uleb128 0x6
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
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
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
	.uleb128 0x6
	.uleb128 0x1
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
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
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
	.uleb128 0x1b
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
	.uleb128 0x1c
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
	.uleb128 0x1d
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
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
	.uleb128 0x21
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
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
	.uleb128 0x23
	.uleb128 0x2e
	.byte	0x0
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
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x25
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
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0xa0
	.value	0x2
	.long	.Ldebug_info0
	.long	0x60c
	.long	0x107
	.string	"itest"
	.long	0x18a
	.string	"ltest"
	.long	0x218
	.string	"ftest"
	.long	0x2ab
	.string	"dtest"
	.long	0x38d
	.string	"cpt"
	.long	0x415
	.string	"run_tests"
	.long	0x4f5
	.string	"main"
	.long	0x52c
	.string	"ival"
	.long	0x559
	.string	"lval"
	.long	0x586
	.string	"fval"
	.long	0x5b3
	.string	"dval"
	.long	0x5c9
	.string	"num_type"
	.long	0x5df
	.string	"den_type"
	.long	0x5f5
	.string	"use_cond"
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
	.quad	.LBB12-.Ltext0
	.quad	.LBE12-.Ltext0
	.quad	.LBB13-.Ltext0
	.quad	.LBE13-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF19:
	.string	"use_cond"
.LASF39:
	.string	"__fmt"
.LASF33:
	.string	"fval"
.LASF25:
	.string	"float"
.LASF17:
	.string	"num_type"
.LASF23:
	.string	"argspec"
.LASF40:
	.string	"init"
.LASF10:
	.string	"NEG_VAL"
.LASF14:
	.string	"SINK_VAL"
.LASF29:
	.string	"run_tests"
.LASF20:
	.string	"sink"
.LASF13:
	.string	"RMIX_VAL"
.LASF1:
	.string	"unsigned char"
.LASF32:
	.string	"lval"
.LASF0:
	.string	"long unsigned int"
.LASF2:
	.string	"short unsigned int"
.LASF35:
	.string	"GNU C 4.3.2"
.LASF28:
	.string	"params"
.LASF27:
	.string	"double"
.LASF22:
	.string	"ltest"
.LASF12:
	.string	"AMIX_VAL"
.LASF15:
	.string	"test_funct"
.LASF26:
	.string	"dtest"
.LASF36:
	.string	"div.c"
.LASF41:
	.string	"main"
.LASF3:
	.string	"unsigned int"
.LASF30:
	.string	"descr"
.LASF16:
	.string	"val_t"
.LASF8:
	.string	"long long unsigned int"
.LASF34:
	.string	"dval"
.LASF37:
	.string	"/home/rbryant/ics2/code/opt"
.LASF31:
	.string	"ival"
.LASF9:
	.string	"long long int"
.LASF21:
	.string	"itest"
.LASF7:
	.string	"char"
.LASF24:
	.string	"ftest"
.LASF38:
	.string	"printf"
.LASF5:
	.string	"short int"
.LASF6:
	.string	"long int"
.LASF11:
	.string	"POS_VAL"
.LASF4:
	.string	"signed char"
.LASF18:
	.string	"den_type"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
