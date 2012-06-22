	.file	"goodcnt.c"
	.text
.globl thread
	.type	thread, @function
thread:
.LFB83:
	pushq	%rbp
.LCFI0:
	pushq	%rbx
.LCFI1:
	subq	$8, %rsp
.LCFI2:
	movl	(%rdi), %ebp
	testl	%ebp, %ebp
	jle	.L2
	movl	$0, %ebx
.L3:
	movl	$mutex, %edi
	call	P
	movl	cnt(%rip), %eax
	addl	$1, %eax
	movl	%eax, cnt(%rip)
	movl	$mutex, %edi
	call	V
	addl	$1, %ebx
	cmpl	%ebx, %ebp
	jg	.L3
.L2:
	movl	$0, %eax
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.LFE83:
	.size	thread, .-thread
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"usage: %s <niters>\n"
.LC1:
	.string	"BOOM! cnt=%d\n"
.LC2:
	.string	"OK cnt=%d\n"
	.text
.globl main
	.type	main, @function
main:
.LFB82:
	pushq	%rbx
.LCFI3:
	subq	$32, %rsp
.LCFI4:
	cmpl	$2, %edi
	je	.L7
	movq	(%rsi), %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movl	$0, %edi
	call	exit
.L7:
	movq	8(%rsi), %rdi
	movl	$10, %edx
	movl	$0, %esi
	call	strtol
	movl	%eax, 28(%rsp)
	movl	$1, %edx
	movl	$0, %esi
	movl	$mutex, %edi
	call	Sem_init
	leaq	28(%rsp), %rbx
	leaq	16(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$thread, %edx
	movl	$0, %esi
	call	Pthread_create
	leaq	8(%rsp), %rdi
	movq	%rbx, %rcx
	movl	$thread, %edx
	movl	$0, %esi
	call	Pthread_create
	movq	16(%rsp), %rdi
	movl	$0, %esi
	call	Pthread_join
	movq	8(%rsp), %rdi
	movl	$0, %esi
	call	Pthread_join
	movl	cnt(%rip), %edx
	movl	28(%rsp), %eax
	addl	%eax, %eax
	cmpl	%edx, %eax
	je	.L8
	movl	cnt(%rip), %edx
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	jmp	.L9
.L8:
	movl	cnt(%rip), %edx
	movl	$.LC2, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
.L9:
	movl	$0, %edi
	call	exit
.LFE82:
	.size	main, .-main
.globl cnt
	.bss
	.align 4
	.type	cnt, @object
	.size	cnt, 4
cnt:
	.zero	4
	.comm	mutex,32,32
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
	.long	.LFB83
	.long	.LFE83-.LFB83
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB83
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
	.byte	0x83
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB82
	.long	.LFE82-.LFB82
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB82
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE3:
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
