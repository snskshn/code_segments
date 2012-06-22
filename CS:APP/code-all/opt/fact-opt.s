	.file	"fact.c"
	.text
	.p2align 4,,15
.globl rfact
	.type	rfact, @function
rfact:
.LFB24:
	cmpl	$1, %edi
	movl	$1, %eax
	jg	.L4
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L7:
	movl	%edx, %edi
.L4:
	leal	-1(%rdi), %edx
	imull	%edi, %eax
	cmpl	$1, %edx
	jg	.L7
.L3:
	rep
	ret
.LFE24:
	.size	rfact, .-rfact
	.p2align 4,,15
.globl fact
	.type	fact, @function
fact:
.LFB25:
	cmpl	$1, %edi
	movl	$2, %edx
	movl	$1, %eax
	jle	.L10
	.p2align 4,,10
	.p2align 3
.L13:
	imull	%edx, %eax
	addl	$1, %edx
	cmpl	%edx, %edi
	jge	.L13
.L10:
	rep
	ret
.LFE25:
	.size	fact, .-fact
	.p2align 4,,15
.globl fact_u2a
	.type	fact_u2a, @function
fact_u2a:
.LFB26:
	cmpl	$2, %edi
	movl	$2, %edx
	movl	$1, %ecx
	movl	$2, %eax
	jle	.L17
	.p2align 4,,10
	.p2align 3
.L23:
	leal	1(%rdx), %eax
	imull	%edx, %eax
	addl	$2, %edx
	imull	%eax, %ecx
	cmpl	%edx, %edi
	jg	.L23
	leal	-3(%rdi), %eax
	shrl	%eax
	leal	4(%rax,%rax), %eax
.L17:
	cmpl	%edi, %eax
	jg	.L19
	.p2align 4,,10
	.p2align 3
.L22:
	imull	%eax, %ecx
	addl	$1, %eax
	cmpl	%eax, %edi
	jge	.L22
.L19:
	movl	%ecx, %eax
	ret
.LFE26:
	.size	fact_u2a, .-fact_u2a
	.p2align 4,,15
.globl fact_u2b
	.type	fact_u2b, @function
fact_u2b:
.LFB27:
	cmpl	$2, %edi
	movl	$2, %edx
	movl	$1, %ecx
	movl	$2, %eax
	jle	.L28
	.p2align 4,,10
	.p2align 3
.L34:
	leal	1(%rdx), %eax
	imull	%edx, %eax
	addl	$2, %edx
	imull	%eax, %ecx
	cmpl	%edx, %edi
	jg	.L34
	leal	-3(%rdi), %eax
	shrl	%eax
	leal	4(%rax,%rax), %eax
.L28:
	cmpl	%edi, %eax
	jg	.L30
	.p2align 4,,10
	.p2align 3
.L33:
	imull	%eax, %ecx
	addl	$1, %eax
	cmpl	%eax, %edi
	jge	.L33
.L30:
	movl	%ecx, %eax
	ret
.LFE27:
	.size	fact_u2b, .-fact_u2b
	.p2align 4,,15
.globl fact_u2c
	.type	fact_u2c, @function
fact_u2c:
.LFB28:
	cmpl	$2, %edi
	jle	.L48
	movl	$2, %edx
	movl	$1, %ecx
	movl	$1, %esi
	.p2align 4,,10
	.p2align 3
.L40:
	leal	1(%rdx), %eax
	imull	%edx, %ecx
	addl	$2, %edx
	imull	%eax, %esi
	cmpl	%edx, %edi
	jg	.L40
	leal	-3(%rdi), %eax
	shrl	%eax
	leal	4(%rax,%rax), %eax
	jmp	.L47
	.p2align 4,,10
	.p2align 3
.L44:
	imull	%eax, %ecx
	addl	$1, %eax
.L47:
	cmpl	%eax, %edi
	jge	.L44
	imull	%esi, %ecx
	movl	%ecx, %eax
	ret
.L48:
	movl	$1, %esi
	movl	$2, %eax
	movl	$1, %ecx
	jmp	.L47
.LFE28:
	.size	fact_u2c, .-fact_u2c
	.p2align 4,,15
.globl fact_u3a
	.type	fact_u3a, @function
fact_u3a:
.LFB29:
	leal	-1(%rdi), %esi
	movl	$2, %edx
	movl	$1, %ecx
	movl	$2, %eax
	cmpl	$2, %esi
	jle	.L51
	.p2align 4,,10
	.p2align 3
.L57:
	imull	%edx, %ecx
	leal	1(%rdx), %eax
	imull	%eax, %ecx
	leal	2(%rdx), %eax
	addl	$3, %edx
	imull	%eax, %ecx
	cmpl	%esi, %edx
	jl	.L57
	leal	-4(%rdi), %edx
	movl	$-1431655765, %eax
	mull	%edx
	shrl	%edx
	leal	5(%rdx,%rdx,2), %eax
.L51:
	cmpl	%edi, %eax
	jg	.L53
	.p2align 4,,10
	.p2align 3
.L56:
	imull	%eax, %ecx
	addl	$1, %eax
	cmpl	%eax, %edi
	jge	.L56
.L53:
	movl	%ecx, %eax
	ret
.LFE29:
	.size	fact_u3a, .-fact_u3a
	.p2align 4,,15
.globl fact_u3b
	.type	fact_u3b, @function
fact_u3b:
.LFB30:
	leal	-1(%rdi), %esi
	movl	$2, %edx
	movl	$1, %ecx
	movl	$2, %eax
	cmpl	$2, %esi
	jle	.L62
	.p2align 4,,10
	.p2align 3
.L68:
	imull	%edx, %ecx
	leal	1(%rdx), %eax
	imull	%eax, %ecx
	leal	2(%rdx), %eax
	addl	$3, %edx
	imull	%eax, %ecx
	cmpl	%esi, %edx
	jl	.L68
	leal	-4(%rdi), %edx
	movl	$-1431655765, %eax
	mull	%edx
	shrl	%edx
	leal	5(%rdx,%rdx,2), %eax
.L62:
	cmpl	%edi, %eax
	jg	.L64
	.p2align 4,,10
	.p2align 3
.L67:
	imull	%eax, %ecx
	addl	$1, %eax
	cmpl	%eax, %edi
	jge	.L67
.L64:
	movl	%ecx, %eax
	ret
.LFE30:
	.size	fact_u3b, .-fact_u3b
	.p2align 4,,15
.globl fact_u3c
	.type	fact_u3c, @function
fact_u3c:
.LFB31:
	leal	-1(%rdi), %r9d
	cmpl	$2, %r9d
	jle	.L82
	movl	$2, %edx
	movl	$1, %ecx
	movl	$1, %r8d
	movl	$1, %esi
	.p2align 4,,10
	.p2align 3
.L74:
	leal	1(%rdx), %eax
	imull	%edx, %ecx
	imull	%eax, %r8d
	leal	2(%rdx), %eax
	addl	$3, %edx
	imull	%eax, %esi
	cmpl	%r9d, %edx
	jl	.L74
	leal	-4(%rdi), %edx
	movl	$-1431655765, %eax
	mull	%edx
	shrl	%edx
	leal	5(%rdx,%rdx,2), %eax
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L78:
	imull	%eax, %ecx
	addl	$1, %eax
.L81:
	cmpl	%eax, %edi
	jge	.L78
	imull	%r8d, %esi
	imull	%ecx, %esi
	movl	%esi, %eax
	ret
.L82:
	movl	$1, %r8d
	movl	$1, %esi
	movl	$2, %eax
	movl	$1, %ecx
	jmp	.L81
.LFE31:
	.size	fact_u3c, .-fact_u3c
	.p2align 4,,15
	.type	test, @function
test:
.LFB33:
	movslq	idx(%rip),%rax
	movq	funs(,%rax,8), %r11
	jmp	*%r11
.LFE33:
	.size	test, .-test
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Description: %s, "
.LC1:
	.string	"CPE = %.2f\n"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"Oops.  %d! yields %d, should be %d\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB36:
	pushq	%r15
.LCFI0:
	pushq	%r14
.LCFI1:
	xorl	%r14d, %r14d
	pushq	%r13
.LCFI2:
	xorl	%r13d, %r13d
	pushq	%r12
.LCFI3:
	pushq	%rbp
.LCFI4:
	pushq	%rbx
.LCFI5:
	subq	$8, %rsp
.LCFI6:
	.p2align 4,,10
	.p2align 3
.L95:
	movq	test_descr(%r13), %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	xorl	%ebx, %ebx
	call	__printf_chk
	movl	$1024, %esi
	movl	$test, %edi
	movl	%r14d, idx(%rip)
	call	find_cpe
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
	movq	funs(%r13), %r12
	.p2align 4,,10
	.p2align 3
.L94:
	movl	%ebx, %edi
	call	*%r12
	cmpl	$1, %ebx
	movl	$1, %esi
	jle	.L87
	movl	%ebx, %edx
	jmp	.L88
	.p2align 4,,10
	.p2align 3
.L99:
	movl	%ecx, %edx
.L88:
	leal	-1(%rdx), %ecx
	imull	%edx, %esi
	cmpl	$1, %ecx
	jg	.L99
.L87:
	cmpl	%esi, %eax
	jne	.L100
	addl	$1, %ebx
	cmpl	$10, %ebx
	jne	.L94
	addl	$1, %r14d
	addq	$8, %r13
	cmpl	$9, %r14d
	jne	.L95
.L102:
	addq	$8, %rsp
	xorl	%eax, %eax
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.L100:
	cmpl	$1, %ebx
	movl	%ebx, %r15d
	movl	$1, %ebp
	jg	.L92
	jmp	.L91
	.p2align 4,,10
	.p2align 3
.L101:
	movl	%eax, %ebx
.L92:
	leal	-1(%rbx), %eax
	imull	%ebx, %ebp
	cmpl	$1, %eax
	jg	.L101
.L91:
	movl	%r15d, %edi
	addl	$1, %r14d
	addq	$8, %r13
	call	*%r12
	movl	%ebp, %r8d
	movl	%eax, %ecx
	movl	%r15d, %edx
	xorl	%eax, %eax
	movl	$.LC2, %esi
	movl	$1, %edi
	call	__printf_chk
	cmpl	$9, %r14d
	jne	.L95
	jmp	.L102
.LFE36:
	.size	main, .-main
	.p2align 4,,15
.globl rfact_a
	.type	rfact_a, @function
rfact_a:
.LFB32:
	movq	%rbx, -48(%rsp)
.LCFI7:
	movq	%rbp, -40(%rsp)
.LCFI8:
	movl	%edi, %ebx
	movq	%r12, -32(%rsp)
.LCFI9:
	movq	%r13, -24(%rsp)
.LCFI10:
	movl	$1, %eax
	movq	%r14, -16(%rsp)
.LCFI11:
	movq	%r15, -8(%rsp)
.LCFI12:
	subq	$72, %rsp
.LCFI13:
	cmpl	$1, %edi
	jle	.L105
	leal	-1(%rbx), %ebp
	movl	$-1, %eax
	cmpl	$1, %ebp
	je	.L107
	leal	-2(%rbx), %r12d
	movl	$-1, %eax
	cmpl	$1, %r12d
	je	.L109
	leal	-3(%rbx), %r13d
	movl	$-1, %eax
	cmpl	$1, %r13d
	je	.L111
	leal	-4(%rbx), %r14d
	movl	$-1, %eax
	cmpl	$1, %r14d
	je	.L113
	leal	-5(%rbx), %r15d
	movl	$-1, %eax
	cmpl	$1, %r15d
	je	.L115
	leal	-6(%rbx), %r8d
	movl	$-1, %eax
	cmpl	$1, %r8d
	je	.L117
	leal	-7(%rbx), %r9d
	movl	$-1, %eax
	cmpl	$1, %r9d
	je	.L119
	leal	-8(%rbx), %r10d
	movl	$-1, %eax
	cmpl	$1, %r10d
	je	.L121
	leal	-9(%rbx), %edi
	movl	%r8d, 16(%rsp)
	movl	%r9d, 8(%rsp)
	movl	%r10d, (%rsp)
	call	rfact_a
	movl	(%rsp), %r10d
	negl	%eax
	movl	8(%rsp), %r9d
	movl	16(%rsp), %r8d
	imull	%r10d, %eax
	negl	%eax
.L121:
	imull	%r9d, %eax
	negl	%eax
.L119:
	imull	%r8d, %eax
	negl	%eax
.L117:
	imull	%r15d, %eax
	negl	%eax
.L115:
	imull	%r14d, %eax
	negl	%eax
.L113:
	imull	%r13d, %eax
	negl	%eax
.L111:
	imull	%r12d, %eax
	negl	%eax
.L109:
	imull	%ebp, %eax
	negl	%eax
.L107:
	imull	%ebx, %eax
.L105:
	movq	24(%rsp), %rbx
	movq	32(%rsp), %rbp
	movq	40(%rsp), %r12
	movq	48(%rsp), %r13
	movq	56(%rsp), %r14
	movq	64(%rsp), %r15
	addq	$72, %rsp
	ret
.LFE32:
	.size	rfact_a, .-rfact_a
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
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x8d
	.uleb128 0x4
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
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB32
	.byte	0x86
	.uleb128 0x6
	.byte	0x83
	.uleb128 0x7
	.byte	0x4
	.long	.LCFI13-.LCFI8
	.byte	0xe
	.uleb128 0x50
	.byte	0x8f
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x5
	.align 8
.LEFDE21:
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
