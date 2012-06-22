	.file	"copy.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
.globl write_read
	.type	write_read, @function
write_read:
.LFB24:
	.file 1 "copy.c"
	.loc 1 14 0
.LVL0:
	.loc 1 18 0
	testl	%edx, %edx
	je	.L4
	movl	$0, %eax
.LVL1:
.L3:
	.loc 1 19 0
	movl	%eax, (%rsi)
	.loc 1 20 0
	movl	(%rdi), %eax
.LVL2:
	addl	$1, %eax
.LVL3:
	.loc 1 18 0
	subl	$1, %edx
.LVL4:
	jne	.L3
.LVL5:
.L4:
	rep
	ret
.LFE24:
	.size	write_read, .-write_read
.globl copy_array
	.type	copy_array, @function
copy_array:
.LFB25:
	.loc 1 28 0
.LVL6:
	.loc 1 30 0
	testl	%edx, %edx
	jle	.L9
	movl	$0, %ecx
.L8:
	.loc 1 31 0
	movl	(%rdi,%rcx,4), %eax
	movl	%eax, (%rsi,%rcx,4)
	addq	$1, %rcx
	.loc 1 30 0
	cmpl	%ecx, %edx
	jg	.L8
.L9:
	rep
	ret
.LFE25:
	.size	copy_array, .-copy_array
.globl clear_array
	.type	clear_array, @function
clear_array:
.LFB26:
	.loc 1 37 0
.LVL7:
	.loc 1 39 0
	testl	%esi, %esi
	jle	.L14
	movl	$0, %eax
.L13:
	.loc 1 40 0
	movl	$0, (%rdi,%rax,4)
	addq	$1, %rax
	.loc 1 39 0
	cmpl	%eax, %esi
	jg	.L13
.L14:
	rep
	ret
.LFE26:
	.size	clear_array, .-clear_array
.globl clear_array_4
	.type	clear_array_4, @function
clear_array_4:
.LFB27:
	.loc 1 46 0
.LVL8:
	.loc 1 48 0
	leal	-3(%rsi), %ecx
.LVL9:
	.loc 1 49 0
	testl	%ecx, %ecx
	jle	.L19
	movl	$0, %edx
.LVL10:
.L18:
	.loc 1 50 0
	movslq	%edx,%rax
	movl	$0, (%rdi,%rax,4)
	.loc 1 51 0
	movl	$0, 4(%rdi,%rax,4)
	.loc 1 52 0
	movl	$0, 8(%rdi,%rax,4)
	.loc 1 53 0
	movl	$0, 12(%rdi,%rax,4)
	.loc 1 49 0
	addl	$4, %edx
	cmpl	%edx, %ecx
	jg	.L18
.L19:
	rep
	ret
.LFE27:
	.size	clear_array_4, .-clear_array_4
.globl array_copy
	.type	array_copy, @function
array_copy:
.LFB28:
	.loc 1 61 0
.LVL11:
	.loc 1 63 0
	testl	%edx, %edx
	jle	.L24
	movl	$0, %ecx
.L23:
	.loc 1 64 0
	movl	(%rdi,%rcx,4), %eax
	movl	%eax, (%rsi,%rcx,4)
	addq	$1, %rcx
	.loc 1 63 0
	cmpl	%ecx, %edx
	jg	.L23
.L24:
	rep
	ret
.LFE28:
	.size	array_copy, .-array_copy
.globl array_clear
	.type	array_clear, @function
array_clear:
.LFB29:
	.loc 1 69 0
.LVL12:
	.loc 1 72 0
	testl	%edx, %edx
	jle	.L29
	movl	$0, %eax
.L28:
	.loc 1 73 0
	movl	$0, (%rsi,%rax,4)
	addq	$1, %rax
	.loc 1 72 0
	cmpl	%eax, %edx
	jg	.L28
.L29:
	rep
	ret
.LFE29:
	.size	array_clear, .-array_clear
	.type	array_copy_2, @function
array_copy_2:
.LFB31:
	.loc 1 83 0
.LVL13:
	movl	%edx, %r9d
	.loc 1 85 0
	leal	-1(%r9), %r10d
.LVL14:
	.loc 1 86 0
	movl	$0, %r8d
.LVL15:
	testl	%r10d, %r10d
	jle	.L33
.LVL16:
.L39:
	.loc 1 87 0
	movslq	%r8d,%rax
	leaq	0(,%rax,4), %rcx
	movl	(%rdi,%rcx), %edx
	movl	%edx, (%rsi,%rcx)
	.loc 1 88 0
	leaq	4(,%rax,4), %rax
	movl	(%rdi,%rax), %edx
	movl	%edx, (%rsi,%rax)
	.loc 1 86 0
	addl	$2, %r8d
	cmpl	%r8d, %r10d
	jg	.L39
.LVL17:
.L33:
	.loc 1 90 0
	cmpl	%r8d, %r9d
	jle	.L37
.L38:
	.loc 1 91 0
	movslq	%r8d,%rax
.LVL18:
	salq	$2, %rax
.LVL19:
	movl	(%rdi,%rax), %edx
	movl	%edx, (%rsi,%rax)
	.loc 1 90 0
	addl	$1, %r8d
.LVL20:
	cmpl	%r8d, %r9d
	jg	.L38
.LVL21:
.L37:
	rep
	ret
.LFE31:
	.size	array_copy_2, .-array_copy_2
	.type	array_clear_2, @function
array_clear_2:
.LFB32:
	.loc 1 95 0
.LVL22:
	.loc 1 97 0
	leal	-1(%rdx), %edi
.LVL23:
	.loc 1 98 0
	movl	$0, %ecx
.LVL24:
	testl	%edi, %edi
	jle	.L44
.L50:
	.loc 1 99 0
	movslq	%ecx,%rax
	movl	$0, (%rsi,%rax,4)
	.loc 1 100 0
	movl	$0, 4(%rsi,%rax,4)
	.loc 1 98 0
	addl	$2, %ecx
	cmpl	%ecx, %edi
	jg	.L50
.L44:
	.loc 1 102 0
	cmpl	%ecx, %edx
	jle	.L48
.L49:
	.loc 1 103 0
	movslq	%ecx,%rax
.LVL25:
	movl	$0, (%rsi,%rax,4)
	.loc 1 102 0
	addl	$1, %ecx
.LVL26:
	cmpl	%ecx, %edx
	jg	.L49
.L48:
	rep
	ret
.LFE32:
	.size	array_clear_2, .-array_clear_2
	.type	array_copy_4, @function
array_copy_4:
.LFB33:
	.loc 1 107 0
.LVL27:
	movl	%edx, %r9d
	.loc 1 109 0
	leal	-3(%r9), %r10d
.LVL28:
	.loc 1 110 0
	movl	$0, %r8d
.LVL29:
	testl	%r10d, %r10d
	jle	.L55
.LVL30:
.L61:
	.loc 1 111 0
	movslq	%r8d,%rax
	leaq	0(,%rax,4), %rcx
	movl	(%rdi,%rcx), %edx
	movl	%edx, (%rsi,%rcx)
	.loc 1 112 0
	leaq	4(,%rax,4), %rcx
	movl	(%rdi,%rcx), %edx
	movl	%edx, (%rsi,%rcx)
	.loc 1 113 0
	leaq	8(,%rax,4), %rcx
	movl	(%rdi,%rcx), %edx
	movl	%edx, (%rsi,%rcx)
	.loc 1 114 0
	leaq	12(,%rax,4), %rax
	movl	(%rdi,%rax), %edx
	movl	%edx, (%rsi,%rax)
	.loc 1 110 0
	addl	$4, %r8d
	cmpl	%r8d, %r10d
	jg	.L61
.LVL31:
.L55:
	.loc 1 116 0
	cmpl	%r8d, %r9d
	jle	.L59
.L60:
	.loc 1 117 0
	movslq	%r8d,%rax
.LVL32:
	salq	$2, %rax
.LVL33:
	movl	(%rdi,%rax), %edx
	movl	%edx, (%rsi,%rax)
	.loc 1 116 0
	addl	$1, %r8d
.LVL34:
	cmpl	%r8d, %r9d
	jg	.L60
.LVL35:
.L59:
	rep
	ret
.LFE33:
	.size	array_copy_4, .-array_copy_4
	.type	array_clear_3, @function
array_clear_3:
.LFB34:
	.loc 1 121 0
.LVL36:
	.loc 1 123 0
	leal	-2(%rdx), %edi
.LVL37:
	.loc 1 124 0
	movl	$0, %ecx
.LVL38:
	testl	%edi, %edi
	jle	.L66
.L72:
	.loc 1 125 0
	movslq	%ecx,%rax
	movl	$0, (%rsi,%rax,4)
	.loc 1 126 0
	movl	$0, 4(%rsi,%rax,4)
	.loc 1 127 0
	movl	$0, 8(%rsi,%rax,4)
	.loc 1 124 0
	addl	$3, %ecx
	cmpl	%ecx, %edi
	jg	.L72
.L66:
	.loc 1 129 0
	cmpl	%ecx, %edx
	jle	.L70
.L71:
	.loc 1 130 0
	movslq	%ecx,%rax
.LVL39:
	movl	$0, (%rsi,%rax,4)
	.loc 1 129 0
	addl	$1, %ecx
.LVL40:
	cmpl	%ecx, %edx
	jg	.L71
.L70:
	rep
	ret
.LFE34:
	.size	array_clear_3, .-array_clear_3
	.type	array_clear_4, @function
array_clear_4:
.LFB35:
	.loc 1 134 0
.LVL41:
	.loc 1 136 0
	leal	-3(%rdx), %edi
.LVL42:
	.loc 1 137 0
	movl	$0, %ecx
.LVL43:
	testl	%edi, %edi
	jle	.L77
.L83:
	.loc 1 138 0
	movslq	%ecx,%rax
	movl	$0, (%rsi,%rax,4)
	.loc 1 139 0
	movl	$0, 4(%rsi,%rax,4)
	.loc 1 140 0
	movl	$0, 8(%rsi,%rax,4)
	.loc 1 141 0
	movl	$0, 12(%rsi,%rax,4)
	.loc 1 137 0
	addl	$4, %ecx
	cmpl	%ecx, %edi
	jg	.L83
.L77:
	.loc 1 143 0
	cmpl	%ecx, %edx
	jle	.L81
.L82:
	.loc 1 144 0
	movslq	%ecx,%rax
.LVL44:
	movl	$0, (%rsi,%rax,4)
	.loc 1 143 0
	addl	$1, %ecx
.LVL45:
	cmpl	%ecx, %edx
	jg	.L82
.L81:
	rep
	ret
.LFE35:
	.size	array_clear_4, .-array_clear_4
	.type	test2a, @function
test2a:
.LFB36:
	.loc 1 178 0
.LVL46:
	subq	$8, %rsp
.LCFI0:
	.loc 1 179 0
	movslq	idx(%rip),%rax
	movl	%edi, %edx
	movl	$data, %esi
	movq	%rsi, %rdi
.LVL47:
	call	*copier(,%rax,8)
	.loc 1 180 0
	addq	$8, %rsp
	ret
.LFE36:
	.size	test2a, .-test2a
	.type	test2b, @function
test2b:
.LFB37:
	.loc 1 183 0
.LVL48:
	subq	$8, %rsp
.LCFI1:
	.loc 1 184 0
	movslq	idx(%rip),%rax
	movl	%edi, %edx
	movl	$data+4, %esi
	movl	$data, %edi
.LVL49:
	call	*copier(,%rax,8)
	.loc 1 185 0
	addq	$8, %rsp
	ret
.LFE37:
	.size	test2b, .-test2b
	.type	test2c, @function
test2c:
.LFB38:
	.loc 1 188 0
.LVL50:
	subq	$8, %rsp
.LCFI2:
	.loc 1 189 0
	movslq	idx(%rip),%rax
	movl	%edi, %edx
	movl	$data, %esi
	movl	$data+4, %edi
.LVL51:
	call	*copier(,%rax,8)
	.loc 1 190 0
	addq	$8, %rsp
	ret
.LFE38:
	.size	test2c, .-test2c
	.type	test2d, @function
test2d:
.LFB39:
	.loc 1 193 0
.LVL52:
	subq	$8, %rsp
.LCFI3:
	.loc 1 194 0
	movslq	idx(%rip),%rax
	movl	%edi, %edx
	movl	$data2, %esi
	movl	$data, %edi
.LVL53:
	call	*copier(,%rax,8)
	.loc 1 195 0
	addq	$8, %rsp
	ret
.LFE39:
	.size	test2d, .-test2d
	.type	mem_clear, @function
mem_clear:
.LFB30:
	.loc 1 77 0
.LVL54:
	subq	$8, %rsp
.LCFI4:
	movq	%rsi, %rdi
.LVL55:
	.file 2 "/usr/include/bits/string3.h"
	.loc 2 85 0
	movslq	%edx,%rdx
.LVL56:
	salq	$2, %rdx
.LVL57:
	movl	$0, %esi
.LVL58:
	call	memset
.LVL59:
	.loc 1 79 0
	addq	$8, %rsp
	ret
.LFE30:
	.size	mem_clear, .-mem_clear
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Description: %s, %s, "
.LC1:
	.string	"CPE = %.2f\n"
	.text
	.type	run_test, @function
run_test:
.LFB40:
	.loc 1 219 0
.LVL60:
	pushq	%r12
.LCFI5:
	pushq	%rbx
.LCFI6:
	subq	$8, %rsp
.LCFI7:
	movl	%edi, %r12d
.LBB8:
.LBB9:
	.file 3 "/usr/include/bits/stdio2.h"
	.loc 3 105 0
	movslq	%esi,%rbx
.LVL61:
	movq	case_descr(,%rbx,8), %rcx
	movslq	%edi,%rax
.LVL62:
	movq	test_descr(,%rax,8), %rdx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
.LVL63:
	call	__printf_chk
.LBE9:
.LBE8:
	.loc 1 223 0
	movl	%r12d, idx(%rip)
.LVL64:
	.loc 1 224 0
	movq	case_fun(,%rbx,8), %rdi
	movl	$1024, %esi
	call	find_cpe
.LBB10:
.LBB11:
	.loc 3 105 0
	movl	$.LC1, %esi
	movl	$1, %edi
	movl	$1, %eax
	call	__printf_chk
.LBE11:
.LBE10:
	.loc 1 227 0
	addq	$8, %rsp
	popq	%rbx
.LVL65:
	popq	%r12
.LVL66:
	ret
.LFE40:
	.size	run_test, .-run_test
.globl main
	.type	main, @function
main:
.LFB41:
	.loc 1 230 0
	pushq	%rbx
.LCFI8:
	.loc 1 230 0
	movl	$0, %ebx
.LVL67:
	jmp	.L99
.L104:
	.loc 1 238 0
	movl	$0, %eax
	popq	%rbx
.LVL68:
	ret
.LVL69:
.L99:
	.loc 1 235 0
	movl	$0, %esi
	movl	%ebx, %edi
	call	run_test
	movl	$1, %esi
	movl	%ebx, %edi
	call	run_test
	movl	$2, %esi
	movl	%ebx, %edi
	call	run_test
	movl	$3, %esi
	movl	%ebx, %edi
	call	run_test
	.loc 1 233 0
	addl	$1, %ebx
	cmpl	$9, %ebx
	jne	.L99
	jmp	.L104
.LFE41:
	.size	main, .-main
.globl idx
	.bss
	.align 4
	.type	idx, @object
	.size	idx, 4
idx:
	.zero	4
	.section	.rodata.str1.1
.LC2:
	.string	"data --> data"
.LC3:
	.string	"data --> data+1"
.LC4:
	.string	"data+1 --> data"
.LC5:
	.string	"data1 --> data2"
	.section	.rodata
	.align 32
	.type	case_descr, @object
	.size	case_descr, 32
case_descr:
	.quad	.LC2
	.quad	.LC3
	.quad	.LC4
	.quad	.LC5
	.section	.rodata.str1.1
.LC6:
	.string	"Write-read"
.LC7:
	.string	"Array clear"
.LC8:
	.string	"Array clear  x2"
.LC9:
	.string	"Array clear  x3"
.LC10:
	.string	"Array clear  x4"
.LC11:
	.string	"Mem clear"
.LC12:
	.string	"Array copy"
.LC13:
	.string	"Array copy  x2"
.LC14:
	.string	"Array copy  x4"
	.section	.rodata
	.align 32
	.type	test_descr, @object
	.size	test_descr, 72
test_descr:
	.quad	.LC6
	.quad	.LC7
	.quad	.LC8
	.quad	.LC9
	.quad	.LC10
	.quad	.LC11
	.quad	.LC12
	.quad	.LC13
	.quad	.LC14
	.align 32
	.type	case_fun, @object
	.size	case_fun, 32
case_fun:
	.quad	test2a
	.quad	test2b
	.quad	test2c
	.quad	test2d
	.align 32
	.type	copier, @object
	.size	copier, 72
copier:
	.quad	write_read
	.quad	array_clear
	.quad	array_clear_2
	.quad	array_clear_3
	.quad	array_clear_4
	.quad	mem_clear
	.quad	array_copy
	.quad	array_copy_2
	.quad	array_copy_4
	.comm	data,4100,32
	.comm	data2,4100,32
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
	.quad	.LFB24
	.quad	.LFE24-.LFB24
	.align 8
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.quad	.LFB25
	.quad	.LFE25-.LFB25
	.align 8
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.quad	.LFB26
	.quad	.LFE26-.LFB26
	.align 8
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.quad	.LFB27
	.quad	.LFE27-.LFB27
	.align 8
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.quad	.LFB28
	.quad	.LFE28-.LFB28
	.align 8
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.quad	.LFB29
	.quad	.LFE29-.LFB29
	.align 8
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.quad	.LFB31
	.quad	.LFE31-.LFB31
	.align 8
.LEFDE12:
.LSFDE14:
	.long	.LEFDE14-.LASFDE14
.LASFDE14:
	.long	.Lframe0
	.quad	.LFB32
	.quad	.LFE32-.LFB32
	.align 8
.LEFDE14:
.LSFDE16:
	.long	.LEFDE16-.LASFDE16
.LASFDE16:
	.long	.Lframe0
	.quad	.LFB33
	.quad	.LFE33-.LFB33
	.align 8
.LEFDE16:
.LSFDE18:
	.long	.LEFDE18-.LASFDE18
.LASFDE18:
	.long	.Lframe0
	.quad	.LFB34
	.quad	.LFE34-.LFB34
	.align 8
.LEFDE18:
.LSFDE20:
	.long	.LEFDE20-.LASFDE20
.LASFDE20:
	.long	.Lframe0
	.quad	.LFB35
	.quad	.LFE35-.LFB35
	.align 8
.LEFDE20:
.LSFDE22:
	.long	.LEFDE22-.LASFDE22
.LASFDE22:
	.long	.Lframe0
	.quad	.LFB36
	.quad	.LFE36-.LFB36
	.byte	0x4
	.long	.LCFI0-.LFB36
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE22:
.LSFDE24:
	.long	.LEFDE24-.LASFDE24
.LASFDE24:
	.long	.Lframe0
	.quad	.LFB37
	.quad	.LFE37-.LFB37
	.byte	0x4
	.long	.LCFI1-.LFB37
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE24:
.LSFDE26:
	.long	.LEFDE26-.LASFDE26
.LASFDE26:
	.long	.Lframe0
	.quad	.LFB38
	.quad	.LFE38-.LFB38
	.byte	0x4
	.long	.LCFI2-.LFB38
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE26:
.LSFDE28:
	.long	.LEFDE28-.LASFDE28
.LASFDE28:
	.long	.Lframe0
	.quad	.LFB39
	.quad	.LFE39-.LFB39
	.byte	0x4
	.long	.LCFI3-.LFB39
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE28:
.LSFDE30:
	.long	.LEFDE30-.LASFDE30
.LASFDE30:
	.long	.Lframe0
	.quad	.LFB30
	.quad	.LFE30-.LFB30
	.byte	0x4
	.long	.LCFI4-.LFB30
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE30:
.LSFDE32:
	.long	.LEFDE32-.LASFDE32
.LASFDE32:
	.long	.Lframe0
	.quad	.LFB40
	.quad	.LFE40-.LFB40
	.byte	0x4
	.long	.LCFI5-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE32:
.LSFDE34:
	.long	.LEFDE34-.LASFDE34
.LASFDE34:
	.long	.Lframe0
	.quad	.LFB41
	.quad	.LFE41-.LFB41
	.byte	0x4
	.long	.LCFI8-.LFB41
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE34:
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
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
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
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB35
	.long	.LFE35-.LFB35
	.uleb128 0x0
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB36
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB37
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI2-.LFB38
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI3-.LFB39
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB30
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB41
	.long	.LFE41-.LFB41
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB41
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x2
	.align 8
.LEFDE35:
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST1:
	.quad	.LVL0-.Ltext0
	.quad	.LVL4-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL5-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST2:
	.quad	.LVL1-.Ltext0
	.quad	.LVL2-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL3-.Ltext0
	.quad	.LFE24-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	0x0
	.quad	0x0
.LLST9:
	.quad	.LVL13-.Ltext0
	.quad	.LVL16-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL16-.Ltext0
	.quad	.LFE31-.Ltext0
	.value	0x1
	.byte	0x59
	.quad	0x0
	.quad	0x0
.LLST10:
	.quad	.LVL15-.Ltext0
	.quad	.LVL18-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL18-.Ltext0
	.quad	.LVL19-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL20-.Ltext0
	.quad	.LFE31-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST12:
	.quad	.LVL22-.Ltext0
	.quad	.LVL23-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST13:
	.quad	.LVL24-.Ltext0
	.quad	.LVL25-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL25-.Ltext0
	.quad	.LVL26-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL26-.Ltext0
	.quad	.LFE32-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST15:
	.quad	.LVL27-.Ltext0
	.quad	.LVL30-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	.LVL30-.Ltext0
	.quad	.LFE33-.Ltext0
	.value	0x1
	.byte	0x59
	.quad	0x0
	.quad	0x0
.LLST16:
	.quad	.LVL29-.Ltext0
	.quad	.LVL32-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	.LVL32-.Ltext0
	.quad	.LVL33-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL34-.Ltext0
	.quad	.LFE33-.Ltext0
	.value	0x1
	.byte	0x58
	.quad	0x0
	.quad	0x0
.LLST18:
	.quad	.LVL36-.Ltext0
	.quad	.LVL37-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST19:
	.quad	.LVL38-.Ltext0
	.quad	.LVL39-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL39-.Ltext0
	.quad	.LVL40-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL40-.Ltext0
	.quad	.LFE34-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST21:
	.quad	.LVL41-.Ltext0
	.quad	.LVL42-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST22:
	.quad	.LVL43-.Ltext0
	.quad	.LVL44-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	.LVL44-.Ltext0
	.quad	.LVL45-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL45-.Ltext0
	.quad	.LFE35-.Ltext0
	.value	0x1
	.byte	0x52
	.quad	0x0
	.quad	0x0
.LLST23:
	.quad	.LFB36-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LFE36-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST24:
	.quad	.LVL46-.Ltext0
	.quad	.LVL47-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST25:
	.quad	.LFB37-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI1-.Ltext0
	.quad	.LFE37-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST26:
	.quad	.LVL48-.Ltext0
	.quad	.LVL49-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST27:
	.quad	.LFB38-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI2-.Ltext0
	.quad	.LFE38-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST28:
	.quad	.LVL50-.Ltext0
	.quad	.LVL51-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST29:
	.quad	.LFB39-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI3-.Ltext0
	.quad	.LFE39-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST30:
	.quad	.LVL52-.Ltext0
	.quad	.LVL53-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST31:
	.quad	.LFB30-.Ltext0
	.quad	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI4-.Ltext0
	.quad	.LFE30-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST32:
	.quad	.LVL54-.Ltext0
	.quad	.LVL55-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST33:
	.quad	.LVL54-.Ltext0
	.quad	.LVL58-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL58-.Ltext0
	.quad	.LVL59-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	0x0
	.quad	0x0
.LLST34:
	.quad	.LVL54-.Ltext0
	.quad	.LVL57-.Ltext0
	.value	0x1
	.byte	0x51
	.quad	0x0
	.quad	0x0
.LLST35:
	.quad	.LFB40-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI5-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI6-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 24
	.quad	.LCFI7-.Ltext0
	.quad	.LFE40-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 32
	.quad	0x0
	.quad	0x0
.LLST36:
	.quad	.LVL60-.Ltext0
	.quad	.LVL62-.Ltext0
	.value	0x1
	.byte	0x55
	.quad	.LVL62-.Ltext0
	.quad	.LVL63-.Ltext0
	.value	0x1
	.byte	0x50
	.quad	.LVL64-.Ltext0
	.quad	.LVL66-.Ltext0
	.value	0x1
	.byte	0x5c
	.quad	0x0
	.quad	0x0
.LLST37:
	.quad	.LVL60-.Ltext0
	.quad	.LVL61-.Ltext0
	.value	0x1
	.byte	0x54
	.quad	.LVL61-.Ltext0
	.quad	.LVL65-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
.LLST38:
	.quad	.LFB41-.Ltext0
	.quad	.LCFI8-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI8-.Ltext0
	.quad	.LFE41-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST39:
	.quad	.LVL67-.Ltext0
	.quad	.LVL68-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	.LVL69-.Ltext0
	.quad	.LFE41-.Ltext0
	.value	0x1
	.byte	0x53
	.quad	0x0
	.quad	0x0
	.file 4 "/usr/lib/gcc/x86_64-linux-gnu/4.3.2/include/stddef.h"
	.section	.debug_info
	.long	0x7b3
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF44
	.byte	0x1
	.long	.LASF45
	.long	.LASF46
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF8
	.byte	0x4
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
	.uleb128 0x7
	.byte	0x8
	.long	0x7b
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x7
	.byte	0x8
	.long	0x88
	.uleb128 0x8
	.long	0x7b
	.uleb128 0x7
	.byte	0x8
	.long	0x93
	.uleb128 0x9
	.byte	0x1
	.long	0x9f
	.uleb128 0xa
	.long	0x62
	.byte	0x0
	.uleb128 0x2
	.long	.LASF9
	.byte	0x1
	.byte	0x93
	.long	0xaa
	.uleb128 0x7
	.byte	0x8
	.long	0xb0
	.uleb128 0x9
	.byte	0x1
	.long	0xc6
	.uleb128 0xa
	.long	0xc6
	.uleb128 0xa
	.long	0xc6
	.uleb128 0xa
	.long	0x62
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x62
	.uleb128 0x2
	.long	.LASF10
	.byte	0x1
	.byte	0xc8
	.long	0x8d
	.uleb128 0xb
	.byte	0x1
	.long	.LASF14
	.byte	0x2
	.byte	0x4f
	.byte	0x1
	.long	0x73
	.byte	0x2
	.long	0x10b
	.uleb128 0xc
	.long	.LASF11
	.byte	0x2
	.byte	0x4e
	.long	0x73
	.uleb128 0xc
	.long	.LASF12
	.byte	0x2
	.byte	0x4e
	.long	0x62
	.uleb128 0xc
	.long	.LASF13
	.byte	0x2
	.byte	0x4e
	.long	0x2d
	.byte	0x0
	.uleb128 0xb
	.byte	0x1
	.long	.LASF15
	.byte	0x3
	.byte	0x68
	.byte	0x1
	.long	0x62
	.byte	0x2
	.long	0x12a
	.uleb128 0xc
	.long	.LASF16
	.byte	0x3
	.byte	0x67
	.long	0x82
	.uleb128 0xd
	.byte	0x0
	.uleb128 0xe
	.byte	0x1
	.long	.LASF18
	.byte	0x1
	.byte	0xe
	.byte	0x1
	.quad	.LFB24
	.quad	.LFE24
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x18c
	.uleb128 0xf
	.string	"src"
	.byte	0x1
	.byte	0xd
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0xd
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0xd
	.long	0x62
	.long	.LLST1
	.uleb128 0x12
	.string	"cnt"
	.byte	0x1
	.byte	0xf
	.long	0x62
	.uleb128 0x13
	.string	"val"
	.byte	0x1
	.byte	0x10
	.long	0x62
	.long	.LLST2
	.byte	0x0
	.uleb128 0xe
	.byte	0x1
	.long	.LASF19
	.byte	0x1
	.byte	0x1c
	.byte	0x1
	.quad	.LFB25
	.quad	.LFE25
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x1db
	.uleb128 0xf
	.string	"src"
	.byte	0x1
	.byte	0x1b
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x1b
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x1b
	.long	0x62
	.byte	0x1
	.byte	0x51
	.uleb128 0x12
	.string	"i"
	.byte	0x1
	.byte	0x1d
	.long	0x62
	.byte	0x0
	.uleb128 0xe
	.byte	0x1
	.long	.LASF20
	.byte	0x1
	.byte	0x25
	.byte	0x1
	.quad	.LFB26
	.quad	.LFE26
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x21d
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x25
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x25
	.long	0x62
	.byte	0x1
	.byte	0x54
	.uleb128 0x12
	.string	"i"
	.byte	0x1
	.byte	0x26
	.long	0x62
	.byte	0x0
	.uleb128 0xe
	.byte	0x1
	.long	.LASF21
	.byte	0x1
	.byte	0x2e
	.byte	0x1
	.quad	.LFB27
	.quad	.LFE27
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x26e
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x2e
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x2e
	.long	0x62
	.byte	0x1
	.byte	0x54
	.uleb128 0x14
	.string	"i"
	.byte	0x1
	.byte	0x2f
	.long	0x62
	.byte	0x1
	.byte	0x51
	.uleb128 0x15
	.long	.LASF22
	.byte	0x1
	.byte	0x30
	.long	0x62
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xe
	.byte	0x1
	.long	.LASF23
	.byte	0x1
	.byte	0x3d
	.byte	0x1
	.quad	.LFB28
	.quad	.LFE28
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x2bd
	.uleb128 0xf
	.string	"src"
	.byte	0x1
	.byte	0x3c
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x3c
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x3c
	.long	0x62
	.byte	0x1
	.byte	0x51
	.uleb128 0x12
	.string	"i"
	.byte	0x1
	.byte	0x3e
	.long	0x62
	.byte	0x0
	.uleb128 0xe
	.byte	0x1
	.long	.LASF24
	.byte	0x1
	.byte	0x45
	.byte	0x1
	.quad	.LFB29
	.quad	.LFE29
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x30c
	.uleb128 0xf
	.string	"src"
	.byte	0x1
	.byte	0x44
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x44
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x44
	.long	0x62
	.byte	0x1
	.byte	0x51
	.uleb128 0x12
	.string	"i"
	.byte	0x1
	.byte	0x46
	.long	0x62
	.byte	0x0
	.uleb128 0x16
	.long	.LASF25
	.byte	0x1
	.byte	0x53
	.byte	0x1
	.quad	.LFB31
	.quad	.LFE31
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x36d
	.uleb128 0xf
	.string	"src"
	.byte	0x1
	.byte	0x52
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x52
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0x52
	.long	0x62
	.long	.LLST9
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x54
	.long	0x62
	.long	.LLST10
	.uleb128 0x14
	.string	"len"
	.byte	0x1
	.byte	0x55
	.long	0x62
	.byte	0x1
	.byte	0x5a
	.byte	0x0
	.uleb128 0x16
	.long	.LASF26
	.byte	0x1
	.byte	0x5f
	.byte	0x1
	.quad	.LFB32
	.quad	.LFE32
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x3ce
	.uleb128 0x11
	.string	"src"
	.byte	0x1
	.byte	0x5e
	.long	0xc6
	.long	.LLST12
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x5e
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x5e
	.long	0x62
	.byte	0x1
	.byte	0x51
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x60
	.long	0x62
	.long	.LLST13
	.uleb128 0x14
	.string	"len"
	.byte	0x1
	.byte	0x61
	.long	0x62
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x16
	.long	.LASF27
	.byte	0x1
	.byte	0x6b
	.byte	0x1
	.quad	.LFB33
	.quad	.LFE33
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x42f
	.uleb128 0xf
	.string	"src"
	.byte	0x1
	.byte	0x6a
	.long	0xc6
	.byte	0x1
	.byte	0x55
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x6a
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0x6a
	.long	0x62
	.long	.LLST15
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x6c
	.long	0x62
	.long	.LLST16
	.uleb128 0x14
	.string	"len"
	.byte	0x1
	.byte	0x6d
	.long	0x62
	.byte	0x1
	.byte	0x5a
	.byte	0x0
	.uleb128 0x16
	.long	.LASF28
	.byte	0x1
	.byte	0x79
	.byte	0x1
	.quad	.LFB34
	.quad	.LFE34
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x490
	.uleb128 0x11
	.string	"src"
	.byte	0x1
	.byte	0x78
	.long	0xc6
	.long	.LLST18
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x78
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x78
	.long	0x62
	.byte	0x1
	.byte	0x51
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x7a
	.long	0x62
	.long	.LLST19
	.uleb128 0x14
	.string	"len"
	.byte	0x1
	.byte	0x7b
	.long	0x62
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x16
	.long	.LASF29
	.byte	0x1
	.byte	0x86
	.byte	0x1
	.quad	.LFB35
	.quad	.LFE35
	.byte	0x2
	.byte	0x77
	.sleb128 8
	.long	0x4f1
	.uleb128 0x11
	.string	"src"
	.byte	0x1
	.byte	0x85
	.long	0xc6
	.long	.LLST21
	.uleb128 0x10
	.long	.LASF17
	.byte	0x1
	.byte	0x85
	.long	0xc6
	.byte	0x1
	.byte	0x54
	.uleb128 0xf
	.string	"n"
	.byte	0x1
	.byte	0x85
	.long	0x62
	.byte	0x1
	.byte	0x51
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0x87
	.long	0x62
	.long	.LLST22
	.uleb128 0x14
	.string	"len"
	.byte	0x1
	.byte	0x88
	.long	0x62
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x17
	.long	.LASF30
	.byte	0x1
	.byte	0xb2
	.byte	0x1
	.quad	.LFB36
	.quad	.LFE36
	.long	.LLST23
	.long	0x51f
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0xb1
	.long	0x62
	.long	.LLST24
	.byte	0x0
	.uleb128 0x17
	.long	.LASF31
	.byte	0x1
	.byte	0xb7
	.byte	0x1
	.quad	.LFB37
	.quad	.LFE37
	.long	.LLST25
	.long	0x54d
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0xb6
	.long	0x62
	.long	.LLST26
	.byte	0x0
	.uleb128 0x17
	.long	.LASF32
	.byte	0x1
	.byte	0xbc
	.byte	0x1
	.quad	.LFB38
	.quad	.LFE38
	.long	.LLST27
	.long	0x57b
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0xbb
	.long	0x62
	.long	.LLST28
	.byte	0x0
	.uleb128 0x17
	.long	.LASF33
	.byte	0x1
	.byte	0xc1
	.byte	0x1
	.quad	.LFB39
	.quad	.LFE39
	.long	.LLST29
	.long	0x5a9
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0xc0
	.long	0x62
	.long	.LLST30
	.byte	0x0
	.uleb128 0x17
	.long	.LASF34
	.byte	0x1
	.byte	0x4d
	.byte	0x1
	.quad	.LFB30
	.quad	.LFE30
	.long	.LLST31
	.long	0x5f5
	.uleb128 0x11
	.string	"src"
	.byte	0x1
	.byte	0x4c
	.long	0xc6
	.long	.LLST32
	.uleb128 0x18
	.long	.LASF17
	.byte	0x1
	.byte	0x4c
	.long	0xc6
	.long	.LLST33
	.uleb128 0x11
	.string	"n"
	.byte	0x1
	.byte	0x4c
	.long	0x62
	.long	.LLST34
	.byte	0x0
	.uleb128 0x17
	.long	.LASF35
	.byte	0x1
	.byte	0xdb
	.byte	0x1
	.quad	.LFB40
	.quad	.LFE40
	.long	.LLST35
	.long	0x679
	.uleb128 0x11
	.string	"i"
	.byte	0x1
	.byte	0xda
	.long	0x62
	.long	.LLST36
	.uleb128 0x11
	.string	"c"
	.byte	0x1
	.byte	0xda
	.long	0x62
	.long	.LLST37
	.uleb128 0x12
	.string	"cpe"
	.byte	0x1
	.byte	0xdc
	.long	0x679
	.uleb128 0x19
	.long	0x10b
	.quad	.LBB8
	.quad	.LBE8
	.byte	0x1
	.byte	0xdd
	.long	0x65b
	.uleb128 0x1a
	.long	0x11d
	.byte	0x0
	.uleb128 0x1b
	.long	0x10b
	.quad	.LBB10
	.quad	.LBE10
	.byte	0x1
	.byte	0xe2
	.uleb128 0x1a
	.long	0x11d
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.byte	0x8
	.byte	0x4
	.long	.LASF36
	.uleb128 0x1c
	.byte	0x1
	.long	.LASF37
	.byte	0x1
	.byte	0xe6
	.long	0x62
	.quad	.LFB41
	.quad	.LFE41
	.long	.LLST38
	.long	0x6bb
	.uleb128 0x13
	.string	"i"
	.byte	0x1
	.byte	0xe7
	.long	0x62
	.long	.LLST39
	.uleb128 0x12
	.string	"c"
	.byte	0x1
	.byte	0xe7
	.long	0x62
	.byte	0x0
	.uleb128 0x1d
	.long	0x9f
	.long	0x6cb
	.uleb128 0x1e
	.long	0x70
	.byte	0x8
	.byte	0x0
	.uleb128 0x15
	.long	.LASF38
	.byte	0x1
	.byte	0x98
	.long	0x6e0
	.byte	0x9
	.byte	0x3
	.quad	copier
	.uleb128 0x8
	.long	0x6bb
	.uleb128 0x1d
	.long	0x75
	.long	0x6f5
	.uleb128 0x1e
	.long	0x70
	.byte	0x8
	.byte	0x0
	.uleb128 0x15
	.long	.LASF39
	.byte	0x1
	.byte	0xa0
	.long	0x70a
	.byte	0x9
	.byte	0x3
	.quad	test_descr
	.uleb128 0x8
	.long	0x6e5
	.uleb128 0x1d
	.long	0xcc
	.long	0x71f
	.uleb128 0x1e
	.long	0x70
	.byte	0x3
	.byte	0x0
	.uleb128 0x15
	.long	.LASF40
	.byte	0x1
	.byte	0xca
	.long	0x734
	.byte	0x9
	.byte	0x3
	.quad	case_fun
	.uleb128 0x8
	.long	0x70f
	.uleb128 0x1d
	.long	0x75
	.long	0x749
	.uleb128 0x1e
	.long	0x70
	.byte	0x3
	.byte	0x0
	.uleb128 0x15
	.long	.LASF41
	.byte	0x1
	.byte	0xd1
	.long	0x75e
	.byte	0x9
	.byte	0x3
	.quad	case_descr
	.uleb128 0x8
	.long	0x739
	.uleb128 0x1d
	.long	0x62
	.long	0x774
	.uleb128 0x1f
	.long	0x70
	.value	0x400
	.byte	0x0
	.uleb128 0x20
	.long	.LASF42
	.byte	0x1
	.byte	0x8
	.long	0x763
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	data
	.uleb128 0x20
	.long	.LASF43
	.byte	0x1
	.byte	0x9
	.long	0x763
	.byte	0x1
	.byte	0x9
	.byte	0x3
	.quad	data2
	.uleb128 0x21
	.string	"idx"
	.byte	0x1
	.byte	0xad
	.long	0x62
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
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x26
	.byte	0x0
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
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
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
	.uleb128 0xf
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
	.uleb128 0x10
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
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
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
	.uleb128 0x17
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
	.uleb128 0x18
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
	.uleb128 0x19
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
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
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
	.uleb128 0x1c
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
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x20
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
	.uleb128 0x21
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
	.long	0x91
	.value	0x2
	.long	.Ldebug_info0
	.long	0x7b7
	.long	0x12a
	.string	"write_read"
	.long	0x18c
	.string	"copy_array"
	.long	0x1db
	.string	"clear_array"
	.long	0x21d
	.string	"clear_array_4"
	.long	0x26e
	.string	"array_copy"
	.long	0x2bd
	.string	"array_clear"
	.long	0x680
	.string	"main"
	.long	0x774
	.string	"data"
	.long	0x78a
	.string	"data2"
	.long	0x7a0
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
	.section	.debug_str,"MS",@progbits,1
.LASF46:
	.string	"/home/rbryant/ics2/code/opt"
.LASF44:
	.string	"GNU C 4.3.2"
.LASF17:
	.string	"dest"
.LASF5:
	.string	"short int"
.LASF8:
	.string	"size_t"
.LASF12:
	.string	"__ch"
.LASF37:
	.string	"main"
.LASF19:
	.string	"copy_array"
.LASF10:
	.string	"test_fun"
.LASF38:
	.string	"copier"
.LASF26:
	.string	"array_clear_2"
.LASF28:
	.string	"array_clear_3"
.LASF23:
	.string	"array_copy"
.LASF29:
	.string	"array_clear_4"
.LASF25:
	.string	"array_copy_2"
.LASF27:
	.string	"array_copy_4"
.LASF22:
	.string	"limit"
.LASF24:
	.string	"array_clear"
.LASF14:
	.string	"memset"
.LASF6:
	.string	"long int"
.LASF11:
	.string	"__dest"
.LASF15:
	.string	"printf"
.LASF20:
	.string	"clear_array"
.LASF18:
	.string	"write_read"
.LASF40:
	.string	"case_fun"
.LASF41:
	.string	"case_descr"
.LASF1:
	.string	"unsigned char"
.LASF4:
	.string	"signed char"
.LASF3:
	.string	"unsigned int"
.LASF30:
	.string	"test2a"
.LASF31:
	.string	"test2b"
.LASF32:
	.string	"test2c"
.LASF33:
	.string	"test2d"
.LASF9:
	.string	"copy_fun"
.LASF39:
	.string	"test_descr"
.LASF34:
	.string	"mem_clear"
.LASF2:
	.string	"short unsigned int"
.LASF7:
	.string	"char"
.LASF42:
	.string	"data"
.LASF21:
	.string	"clear_array_4"
.LASF16:
	.string	"__fmt"
.LASF0:
	.string	"long unsigned int"
.LASF45:
	.string	"copy.c"
.LASF13:
	.string	"__len"
.LASF36:
	.string	"double"
.LASF43:
	.string	"data2"
.LASF35:
	.string	"run_test"
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
