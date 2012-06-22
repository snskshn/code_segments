	.file	"simd_eg.c"
	.text
	.p2align 4,,15
.globl innerv
	.type	innerv, @function
innerv:
.LFB2:
	mulps	%xmm1, %xmm0
	xorl	%edx, %edx
	xorps	%xmm1, %xmm1
	movaps	%xmm0, -24(%rsp)
	.p2align 4,,7
.L5:
	movslq	%edx,%rax
	incl	%edx
	cmpl	$3, %edx
	addss	-24(%rsp,%rax,4), %xmm1
	jbe	.L5
	movaps	%xmm1, %xmm0
	ret
.LFE2:
	.size	innerv, .-innerv
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
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
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.align 8
.LEFDE1:
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.2 20041017 (Red Hat 3.4.2-6.fc3)"
