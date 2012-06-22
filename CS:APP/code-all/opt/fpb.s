	.file	"combine.c"
	.text
	.p2align 4,,15
.globl register_combiners
	.type	register_combiners, @function
register_combiners:
.LFB55:
	movl	$combine1, %esi
	subq	$8, %rsp
.LCFI0:
	movl	$combine1_descr, %edx
	movq	%rsi, %rdi
	call	add_combiner
	movl	$combine2_descr, %edx
	movl	$combine1, %esi
	movl	$combine2, %edi
	call	add_combiner
	movl	$combine3_descr, %edx
	movl	$combine1, %esi
	movl	$combine3, %edi
	call	add_combiner
	movl	$combine3w_descr, %edx
	movl	$combine1, %esi
	movl	$combine3w, %edi
	call	add_combiner
	movl	$combine4_descr, %edx
	movl	$combine1, %esi
	movl	$combine4, %edi
	call	add_combiner
	movl	$combine4b_descr, %edx
	movl	$combine1, %esi
	movl	$combine4b, %edi
	call	add_combiner
	movl	$combine4p_descr, %edx
	movl	$combine1, %esi
	movl	$combine4p, %edi
	call	add_combiner
	movl	$combine5_descr, %edx
	movl	$combine1, %esi
	movl	$combine5, %edi
	call	add_combiner
	movl	$combine5p_descr, %edx
	movl	$combine1, %esi
	movl	$combine5p, %edi
	call	add_combiner
	movl	$unroll2aw_descr, %edx
	movl	$combine1, %esi
	movl	$unroll2aw_combine, %edi
	call	add_combiner
	movl	$unroll3a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3a_combine, %edi
	call	add_combiner
	movl	$unroll4a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4a_combine, %edi
	call	add_combiner
	movl	$unroll5a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll5a_combine, %edi
	call	add_combiner
	movl	$unroll6a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll6a_combine, %edi
	call	add_combiner
	movl	$unroll8a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8a_combine, %edi
	call	add_combiner
	movl	$unroll16a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll16a_combine, %edi
	call	add_combiner
	movl	$unroll2_descr, %edx
	movl	$combine1, %esi
	movl	$unroll2_combine, %edi
	call	add_combiner
	movl	$unroll3_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3_combine, %edi
	call	add_combiner
	movl	$unroll4_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4_combine, %edi
	call	add_combiner
	movl	$unroll8_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8_combine, %edi
	call	add_combiner
	movl	$unroll16_descr, %edx
	movl	$combine1, %esi
	movl	$unroll16_combine, %edi
	call	add_combiner
	movl	$combine6_descr, %edx
	movl	$combine1, %esi
	movl	$combine6, %edi
	call	add_combiner
	movl	$unroll4x2a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4x2a_combine, %edi
	call	add_combiner
	movl	$unroll8x2a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x2a_combine, %edi
	call	add_combiner
	movl	$unroll3x3a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3x3a_combine, %edi
	call	add_combiner
	movl	$unroll4x4a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4x4a_combine, %edi
	call	add_combiner
	movl	$unroll5x5a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll5x5a_combine, %edi
	call	add_combiner
	movl	$unroll6x6a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll6x6a_combine, %edi
	call	add_combiner
	movl	$unroll8x4a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x4a_combine, %edi
	call	add_combiner
	movl	$unroll8x8a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x8a_combine, %edi
	call	add_combiner
	movl	$unroll10x10a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll10x10a_combine, %edi
	call	add_combiner
	movl	$unroll12x6a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll12x6a_combine, %edi
	call	add_combiner
	movl	$unroll12x12a_descr, %edx
	movl	$combine1, %esi
	movl	$unroll12x12a_combine, %edi
	call	add_combiner
	movl	$unroll8x2_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x2_combine, %edi
	call	add_combiner
	movl	$unroll8x4_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x4_combine, %edi
	call	add_combiner
	movl	$unroll8x8_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8x8_combine, %edi
	call	add_combiner
	movl	$unroll9x3_descr, %edx
	movl	$combine1, %esi
	movl	$unroll9x3_combine, %edi
	call	add_combiner
	movl	$unrollx2as_descr, %edx
	movl	$combine1, %esi
	movl	$unrollx2as_combine, %edi
	call	add_combiner
	movl	$combine7_descr, %edx
	movl	$combine1, %esi
	movl	$combine7, %edi
	call	add_combiner
	movl	$unroll3aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll3aa_combine, %edi
	call	add_combiner
	movl	$unroll4aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll4aa_combine, %edi
	call	add_combiner
	movl	$unroll5aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll5aa_combine, %edi
	call	add_combiner
	movl	$unroll6aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll6aa_combine, %edi
	call	add_combiner
	movl	$unroll8aa_descr, %edx
	movl	$combine1, %esi
	movl	$unroll8aa_combine, %edi
	call	add_combiner
	movl	$simd_v1_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v1_combine, %edi
	call	add_combiner
	movl	$simd_v2_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v2_combine, %edi
	call	add_combiner
	movl	$simd_v4_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v4_combine, %edi
	call	add_combiner
	movl	$simd_v8_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v8_combine, %edi
	call	add_combiner
	movl	$simd_v12_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v12_combine, %edi
	call	add_combiner
	movl	$simd_v2a_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v2a_combine, %edi
	call	add_combiner
	movl	$simd_v4a_descr, %edx
	movl	$combine1, %esi
	movl	$simd_v4a_combine, %edi
	call	add_combiner
	movl	$simd_v8a_combine, %edi
	movl	$simd_v8a_descr, %edx
	movl	$combine1, %esi
	call	add_combiner
	movsd	.LC0(%rip), %xmm1
	movl	$simd_v8a_combine, %edi
	movsd	.LC1(%rip), %xmm0
	addq	$8, %rsp
	jmp	log_combiner
.LFE55:
	.size	register_combiners, .-register_combiners
	.p2align 4,,15
.globl simd_v8a_combine
	.type	simd_v8a_combine, @function
simd_v8a_combine:
.LFB54:
	pushq	%r12
.LCFI1:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI2:
	pushq	%rbx
.LCFI3:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI4:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm4
	jne	.L19
.L13:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm3
	.p2align 4,,10
	.p2align 3
.L6:
	cmpl	$31, %esi
	jbe	.L8
	subl	$32, %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$5, %ecx
	mov	%ecx, %eax
	salq	$7, %rax
	leaq	128(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L9:
	movaps	(%rdx), %xmm2
	movaps	32(%rdx), %xmm0
	mulps	16(%rdx), %xmm2
	movaps	64(%rdx), %xmm1
	mulps	48(%rdx), %xmm0
	mulps	80(%rdx), %xmm1
	mulps	%xmm0, %xmm2
	movaps	96(%rdx), %xmm0
	mulps	112(%rdx), %xmm0
	subq	$-128, %rdx
	cmpq	%rax, %rdx
	mulps	%xmm0, %xmm1
	mulps	%xmm1, %xmm2
	mulps	%xmm2, %xmm4
	jne	.L9
	sall	$5, %ecx
	movq	%rdx, %rbp
	subl	%ecx, %esi
.L8:
	testl	%esi, %esi
	je	.L10
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L11:
	mulss	(%rdx), %xmm3
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L11
.L10:
	movaps	%xmm4, 16(%rsp)
	mulss	16(%rsp), %xmm3
	mulss	20(%rsp), %xmm3
	mulss	24(%rsp), %xmm3
	mulss	28(%rsp), %xmm3
	movss	%xmm3, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L19:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm3
	leal	-1(%rax), %edi
	je	.L13
	.p2align 4,,10
	.p2align 3
.L14:
	mulss	(%rbp), %xmm3
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L14
	jmp	.L6
.LFE54:
	.size	simd_v8a_combine, .-simd_v8a_combine
	.p2align 4,,15
.globl simd_v4a_combine
	.type	simd_v4a_combine, @function
simd_v4a_combine:
.LFB53:
	pushq	%r12
.LCFI5:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI6:
	pushq	%rbx
.LCFI7:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI8:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm3
	jne	.L35
.L30:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm2
	.p2align 4,,10
	.p2align 3
.L23:
	cmpl	$15, %esi
	jbe	.L25
	subl	$16, %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$4, %ecx
	mov	%ecx, %eax
	salq	$6, %rax
	leaq	64(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L26:
	movaps	(%rdx), %xmm1
	movaps	32(%rdx), %xmm0
	mulps	16(%rdx), %xmm1
	mulps	48(%rdx), %xmm0
	addq	$64, %rdx
	cmpq	%rax, %rdx
	mulps	%xmm0, %xmm1
	mulps	%xmm1, %xmm3
	jne	.L26
	sall	$4, %ecx
	movq	%rdx, %rbp
	subl	%ecx, %esi
.L25:
	testl	%esi, %esi
	je	.L27
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L28:
	mulss	(%rdx), %xmm2
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L28
.L27:
	movaps	%xmm3, 16(%rsp)
	mulss	16(%rsp), %xmm2
	mulss	20(%rsp), %xmm2
	mulss	24(%rsp), %xmm2
	mulss	28(%rsp), %xmm2
	movss	%xmm2, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L35:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm2
	leal	-1(%rax), %edi
	je	.L30
	.p2align 4,,10
	.p2align 3
.L31:
	mulss	(%rbp), %xmm2
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L31
	jmp	.L23
.LFE53:
	.size	simd_v4a_combine, .-simd_v4a_combine
	.p2align 4,,15
.globl simd_v2a_combine
	.type	simd_v2a_combine, @function
simd_v2a_combine:
.LFB52:
	pushq	%r12
.LCFI9:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI10:
	pushq	%rbx
.LCFI11:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI12:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm2
	jne	.L51
.L46:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm1
	.p2align 4,,10
	.p2align 3
.L39:
	cmpl	$7, %esi
	jbe	.L41
	subl	$8, %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$3, %ecx
	mov	%ecx, %eax
	salq	$5, %rax
	leaq	32(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L42:
	movaps	(%rdx), %xmm0
	mulps	16(%rdx), %xmm0
	addq	$32, %rdx
	cmpq	%rax, %rdx
	mulps	%xmm0, %xmm2
	jne	.L42
	leal	0(,%rcx,8), %eax
	movq	%rdx, %rbp
	subl	%eax, %esi
.L41:
	testl	%esi, %esi
	je	.L43
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L44:
	mulss	(%rdx), %xmm1
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L44
.L43:
	movaps	%xmm2, 16(%rsp)
	mulss	16(%rsp), %xmm1
	mulss	20(%rsp), %xmm1
	mulss	24(%rsp), %xmm1
	mulss	28(%rsp), %xmm1
	movss	%xmm1, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L51:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm1
	leal	-1(%rax), %edi
	je	.L46
	.p2align 4,,10
	.p2align 3
.L47:
	mulss	(%rbp), %xmm1
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L47
	jmp	.L39
.LFE52:
	.size	simd_v2a_combine, .-simd_v2a_combine
	.p2align 4,,15
.globl simd_v12_combine
	.type	simd_v12_combine, @function
simd_v12_combine:
.LFB51:
	pushq	%r12
.LCFI13:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI14:
	pushq	%rbx
.LCFI15:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI16:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm0
	jne	.L68
.L63:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm11
	.p2align 4,,10
	.p2align 3
.L55:
	cmpl	$47, %esi
	jbe	.L69
	movl	$-1431655765, %eax
	subl	$48, %esi
	movq	%rbp, %rdi
	mull	%esi
	movaps	%xmm0, %xmm12
	movaps	%xmm0, %xmm9
	movl	%edx, %ecx
	movaps	%xmm0, %xmm10
	shrl	$5, %ecx
	movaps	%xmm0, %xmm7
	mov	%ecx, %eax
	movaps	%xmm0, %xmm8
	leaq	3(%rax,%rax,2), %rax
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm6
	salq	$6, %rax
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm4
	leaq	(%rbp,%rax), %rax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L59:
	mulps	(%rdi), %xmm0
	mulps	16(%rdi), %xmm12
	mulps	32(%rdi), %xmm9
	mulps	48(%rdi), %xmm10
	mulps	64(%rdi), %xmm7
	mulps	80(%rdi), %xmm8
	mulps	96(%rdi), %xmm5
	mulps	112(%rdi), %xmm6
	mulps	128(%rdi), %xmm3
	mulps	144(%rdi), %xmm4
	mulps	160(%rdi), %xmm1
	mulps	176(%rdi), %xmm2
	addq	$192, %rdi
	cmpq	%rax, %rdi
	jne	.L59
	leal	(%rcx,%rcx,2), %eax
	movq	%rdi, %rbp
	sall	$4, %eax
	subl	%eax, %esi
.L58:
	testl	%esi, %esi
	je	.L60
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L61:
	mulss	(%rdx), %xmm11
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L61
.L60:
	mulps	%xmm12, %xmm0
	mulps	%xmm10, %xmm9
	mulps	%xmm8, %xmm7
	mulps	%xmm6, %xmm5
	mulps	%xmm9, %xmm0
	mulps	%xmm4, %xmm3
	mulps	%xmm2, %xmm1
	mulps	%xmm7, %xmm0
	mulps	%xmm5, %xmm0
	mulps	%xmm3, %xmm0
	mulps	%xmm1, %xmm0
	movaps	%xmm0, 16(%rsp)
	mulss	16(%rsp), %xmm11
	mulss	20(%rsp), %xmm11
	mulss	24(%rsp), %xmm11
	mulss	28(%rsp), %xmm11
	movss	%xmm11, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L68:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm11
	leal	-1(%rax), %edi
	je	.L63
	.p2align 4,,10
	.p2align 3
.L64:
	mulss	(%rbp), %xmm11
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L64
	jmp	.L55
.L69:
	movaps	%xmm0, %xmm12
	movaps	%xmm0, %xmm9
	movaps	%xmm0, %xmm10
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm8
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm6
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm2
	jmp	.L58
.LFE51:
	.size	simd_v12_combine, .-simd_v12_combine
	.p2align 4,,15
.globl simd_v8_combine
	.type	simd_v8_combine, @function
simd_v8_combine:
.LFB50:
	pushq	%r12
.LCFI17:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI18:
	pushq	%rbx
.LCFI19:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI20:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm0
	jne	.L86
.L81:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm8
	.p2align 4,,10
	.p2align 3
.L73:
	cmpl	$31, %esi
	jbe	.L87
	subl	$32, %esi
	movaps	%xmm0, %xmm7
	movl	%esi, %ecx
	movaps	%xmm0, %xmm6
	shrl	$5, %ecx
	movaps	%xmm0, %xmm5
	mov	%ecx, %eax
	movaps	%xmm0, %xmm4
	salq	$7, %rax
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	leaq	128(%rbp,%rax), %rax
	movaps	%xmm0, %xmm1
	movq	%rbp, %rdx
	.p2align 4,,10
	.p2align 3
.L77:
	mulps	(%rdx), %xmm0
	mulps	16(%rdx), %xmm7
	mulps	32(%rdx), %xmm6
	mulps	48(%rdx), %xmm5
	mulps	64(%rdx), %xmm4
	mulps	80(%rdx), %xmm3
	mulps	96(%rdx), %xmm2
	mulps	112(%rdx), %xmm1
	subq	$-128, %rdx
	cmpq	%rax, %rdx
	jne	.L77
	sall	$5, %ecx
	movq	%rdx, %rbp
	subl	%ecx, %esi
.L76:
	testl	%esi, %esi
	je	.L78
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L79:
	mulss	(%rdx), %xmm8
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L79
.L78:
	mulps	%xmm7, %xmm0
	mulps	%xmm5, %xmm6
	mulps	%xmm3, %xmm4
	mulps	%xmm1, %xmm2
	mulps	%xmm6, %xmm0
	mulps	%xmm4, %xmm0
	mulps	%xmm2, %xmm0
	movaps	%xmm0, 16(%rsp)
	mulss	16(%rsp), %xmm8
	mulss	20(%rsp), %xmm8
	mulss	24(%rsp), %xmm8
	mulss	28(%rsp), %xmm8
	movss	%xmm8, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L86:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm8
	leal	-1(%rax), %edi
	je	.L81
	.p2align 4,,10
	.p2align 3
.L82:
	mulss	(%rbp), %xmm8
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L82
	jmp	.L73
.L87:
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm6
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	jmp	.L76
.LFE50:
	.size	simd_v8_combine, .-simd_v8_combine
	.p2align 4,,15
.globl simd_v4_combine
	.type	simd_v4_combine, @function
simd_v4_combine:
.LFB49:
	pushq	%r12
.LCFI21:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI22:
	pushq	%rbx
.LCFI23:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI24:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm1
	jne	.L104
.L99:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm4
	.p2align 4,,10
	.p2align 3
.L91:
	cmpl	$15, %esi
	jbe	.L105
	subl	$16, %esi
	movaps	%xmm1, %xmm3
	movl	%esi, %ecx
	movaps	%xmm1, %xmm2
	shrl	$4, %ecx
	movaps	%xmm1, %xmm0
	mov	%ecx, %eax
	movq	%rbp, %rdx
	salq	$6, %rax
	leaq	64(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L95:
	mulps	(%rdx), %xmm1
	mulps	16(%rdx), %xmm3
	mulps	32(%rdx), %xmm2
	mulps	48(%rdx), %xmm0
	addq	$64, %rdx
	cmpq	%rax, %rdx
	jne	.L95
	sall	$4, %ecx
	movq	%rdx, %rbp
	subl	%ecx, %esi
.L94:
	testl	%esi, %esi
	je	.L96
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L97:
	mulss	(%rdx), %xmm4
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L97
.L96:
	mulps	%xmm3, %xmm1
	mulps	%xmm0, %xmm2
	mulps	%xmm2, %xmm1
	movaps	%xmm1, 16(%rsp)
	mulss	16(%rsp), %xmm4
	mulss	20(%rsp), %xmm4
	mulss	24(%rsp), %xmm4
	mulss	28(%rsp), %xmm4
	movss	%xmm4, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L104:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm4
	leal	-1(%rax), %edi
	je	.L99
	.p2align 4,,10
	.p2align 3
.L100:
	mulss	(%rbp), %xmm4
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L100
	jmp	.L91
.L105:
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L94
.LFE49:
	.size	simd_v4_combine, .-simd_v4_combine
	.p2align 4,,15
.globl simd_v2_combine
	.type	simd_v2_combine, @function
simd_v2_combine:
.LFB48:
	pushq	%r12
.LCFI25:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI26:
	pushq	%rbx
.LCFI27:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI28:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm1
	jne	.L123
.L117:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm2
	.p2align 4,,10
	.p2align 3
.L109:
	cmpl	$7, %esi
	movaps	%xmm1, %xmm0
	movq	%rbp, %rdx
	jbe	.L112
	subl	$8, %esi
	movl	%esi, %ecx
	shrl	$3, %ecx
	mov	%ecx, %eax
	salq	$5, %rax
	leaq	32(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L118:
	mulps	(%rdx), %xmm1
	mulps	16(%rdx), %xmm0
	addq	$32, %rdx
	cmpq	%rax, %rdx
	jne	.L118
	leal	0(,%rcx,8), %eax
	movq	%rdx, %rbp
	subl	%eax, %esi
.L112:
	testl	%esi, %esi
	je	.L114
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L115:
	mulss	(%rdx), %xmm2
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L115
.L114:
	mulps	%xmm0, %xmm1
	movaps	%xmm1, 16(%rsp)
	mulss	16(%rsp), %xmm2
	mulss	20(%rsp), %xmm2
	mulss	24(%rsp), %xmm2
	mulss	28(%rsp), %xmm2
	movss	%xmm2, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L123:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm2
	leal	-1(%rax), %edi
	je	.L117
	.p2align 4,,10
	.p2align 3
.L119:
	mulss	(%rbp), %xmm2
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L119
	jmp	.L109
.LFE48:
	.size	simd_v2_combine, .-simd_v2_combine
	.p2align 4,,15
.globl simd_v1_combine
	.type	simd_v1_combine, @function
simd_v1_combine:
.LFB47:
	pushq	%r12
.LCFI29:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI30:
	pushq	%rbx
.LCFI31:
	movq	%rdi, %rbx
	subq	$32, %rsp
.LCFI32:
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	movl	$0x3f800000, %edx
	testb	$15, %bpl
	movl	%eax, %esi
	movl	%edx, 16(%rsp)
	movl	%edx, 20(%rsp)
	movl	%edx, 24(%rsp)
	movl	%edx, 28(%rsp)
	movaps	16(%rsp), %xmm1
	jne	.L139
.L134:
	movl	%edx, 12(%rsp)
	movss	12(%rsp), %xmm0
	.p2align 4,,10
	.p2align 3
.L127:
	cmpl	$3, %esi
	jbe	.L129
	subl	$4, %esi
	movq	%rbp, %rdx
	movl	%esi, %ecx
	shrl	$2, %ecx
	mov	%ecx, %eax
	salq	$4, %rax
	leaq	16(%rbp,%rax), %rax
	.p2align 4,,10
	.p2align 3
.L130:
	mulps	(%rdx), %xmm1
	addq	$16, %rdx
	cmpq	%rax, %rdx
	jne	.L130
	leal	0(,%rcx,4), %eax
	movq	%rdx, %rbp
	subl	%eax, %esi
.L129:
	testl	%esi, %esi
	je	.L131
	leal	-1(%rsi), %eax
	movq	%rbp, %rdx
	leaq	4(%rbp,%rax,4), %rax
	.p2align 4,,10
	.p2align 3
.L132:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rax, %rdx
	jne	.L132
.L131:
	movaps	%xmm1, 16(%rsp)
	mulss	16(%rsp), %xmm0
	mulss	20(%rsp), %xmm0
	mulss	24(%rsp), %xmm0
	mulss	28(%rsp), %xmm0
	movss	%xmm0, (%r12)
	addq	$32, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L139:
	movl	%edx, 12(%rsp)
	xorl	%ecx, %ecx
	testl	%eax, %eax
	movss	12(%rsp), %xmm0
	leal	-1(%rax), %edi
	je	.L134
	.p2align 4,,10
	.p2align 3
.L135:
	mulss	(%rbp), %xmm0
	addq	$4, %rbp
	subl	$1, %esi
	testb	$15, %bpl
	setne	%dl
	cmpl	%ecx, %edi
	setne	%al
	addq	$1, %rcx
	testb	%al, %dl
	jne	.L135
	jmp	.L127
.LFE47:
	.size	simd_v1_combine, .-simd_v1_combine
	.p2align 4,,15
.globl unroll8aa_combine
	.type	unroll8aa_combine, @function
unroll8aa_combine:
.LFB46:
	pushq	%r13
.LCFI33:
	movq	%rsi, %r13
	pushq	%r12
.LCFI34:
	pushq	%rbp
.LCFI35:
	pushq	%rbx
.LCFI36:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI37:
	call	vec_length
	leaq	-7(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rdx
	jle	.L149
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm3
	.p2align 4,,10
	.p2align 3
.L143:
	movss	(%rdx,%rax,4), %xmm2
	movss	8(%rdx,%rax,4), %xmm0
	mulss	4(%rdx,%rax,4), %xmm2
	mulss	12(%rdx,%rax,4), %xmm0
	movss	16(%rdx,%rax,4), %xmm1
	mulss	20(%rdx,%rax,4), %xmm1
	mulss	%xmm0, %xmm2
	movss	24(%rdx,%rax,4), %xmm0
	mulss	28(%rdx,%rax,4), %xmm0
	addq	$8, %rax
	cmpq	%rax, %r12
	mulss	%xmm0, %xmm1
	mulss	%xmm1, %xmm2
	mulss	%xmm2, %xmm3
	jg	.L143
	leaq	-8(%rbp), %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rcx
.L142:
	cmpq	%rcx, %rbp
	jle	.L144
	leaq	(%rdx,%rcx,4), %rax
	.p2align 4,,10
	.p2align 3
.L145:
	addq	$1, %rcx
	mulss	(%rax), %xmm3
	addq	$4, %rax
	cmpq	%rcx, %rbp
	jg	.L145
.L144:
	movss	%xmm3, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L149:
	xorl	%ecx, %ecx
	movss	.LC2(%rip), %xmm3
	jmp	.L142
.LFE46:
	.size	unroll8aa_combine, .-unroll8aa_combine
	.p2align 4,,15
.globl unroll6aa_combine
	.type	unroll6aa_combine, @function
unroll6aa_combine:
.LFB45:
	pushq	%r13
.LCFI38:
	movq	%rsi, %r13
	pushq	%r12
.LCFI39:
	pushq	%rbp
.LCFI40:
	pushq	%rbx
.LCFI41:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI42:
	call	vec_length
	leaq	-5(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L159
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L153:
	movss	(%rcx,%rax,4), %xmm0
	movss	8(%rcx,%rax,4), %xmm1
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm1
	mulss	%xmm1, %xmm0
	movss	16(%rcx,%rax,4), %xmm1
	mulss	20(%rcx,%rax,4), %xmm1
	addq	$6, %rax
	cmpq	%rax, %r12
	mulss	%xmm1, %xmm0
	mulss	%xmm0, %xmm2
	jg	.L153
	leaq	-6(%rbp), %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	addq	%rdx, %rdx
.L152:
	cmpq	%rdx, %rbp
	jle	.L154
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L155:
	addq	$1, %rdx
	mulss	(%rax), %xmm2
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L155
.L154:
	movss	%xmm2, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L159:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm2
	jmp	.L152
.LFE45:
	.size	unroll6aa_combine, .-unroll6aa_combine
	.p2align 4,,15
.globl unroll5aa_combine
	.type	unroll5aa_combine, @function
unroll5aa_combine:
.LFB44:
	pushq	%r13
.LCFI43:
	movq	%rsi, %r13
	pushq	%r12
.LCFI44:
	pushq	%rbp
.LCFI45:
	pushq	%rbx
.LCFI46:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI47:
	call	vec_length
	leaq	-4(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L169
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L163:
	movss	(%rcx,%rax,4), %xmm0
	movss	8(%rcx,%rax,4), %xmm1
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm1
	mulss	%xmm1, %xmm0
	mulss	16(%rcx,%rax,4), %xmm0
	addq	$5, %rax
	cmpq	%rax, %r12
	mulss	%xmm0, %xmm2
	jg	.L163
	leaq	-5(%rbp), %rdx
	movabsq	$-3689348814741910323, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	5(%rdx,%rdx,4), %rdx
.L162:
	cmpq	%rdx, %rbp
	jle	.L164
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L165:
	addq	$1, %rdx
	mulss	(%rax), %xmm2
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L165
.L164:
	movss	%xmm2, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L169:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm2
	jmp	.L162
.LFE44:
	.size	unroll5aa_combine, .-unroll5aa_combine
	.p2align 4,,15
.globl unroll4aa_combine
	.type	unroll4aa_combine, @function
unroll4aa_combine:
.LFB43:
	pushq	%r13
.LCFI48:
	movq	%rsi, %r13
	pushq	%r12
.LCFI49:
	pushq	%rbp
.LCFI50:
	pushq	%rbx
.LCFI51:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI52:
	call	vec_length
	leaq	-3(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L179
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm2
	.p2align 4,,10
	.p2align 3
.L173:
	movss	(%rcx,%rax,4), %xmm1
	movss	8(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm1
	mulss	12(%rcx,%rax,4), %xmm0
	addq	$4, %rax
	cmpq	%rax, %r12
	mulss	%xmm0, %xmm1
	mulss	%xmm1, %xmm2
	jg	.L173
	leaq	-4(%rbp), %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L172:
	cmpq	%rdx, %rbp
	jle	.L174
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L175:
	addq	$1, %rdx
	mulss	(%rax), %xmm2
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L175
.L174:
	movss	%xmm2, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L179:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm2
	jmp	.L172
.LFE43:
	.size	unroll4aa_combine, .-unroll4aa_combine
	.p2align 4,,15
.globl unroll3aa_combine
	.type	unroll3aa_combine, @function
unroll3aa_combine:
.LFB42:
	pushq	%r13
.LCFI53:
	movq	%rsi, %r13
	pushq	%r12
.LCFI54:
	pushq	%rbp
.LCFI55:
	pushq	%rbx
.LCFI56:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI57:
	call	vec_length
	leaq	-2(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L189
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm1
	.p2align 4,,10
	.p2align 3
.L183:
	movss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	addq	$3, %rax
	cmpq	%rax, %r12
	mulss	%xmm0, %xmm1
	jg	.L183
	leaq	-3(%rbp), %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	%rdx
	leaq	3(%rdx,%rdx,2), %rdx
.L182:
	cmpq	%rdx, %rbp
	jle	.L184
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L185:
	addq	$1, %rdx
	mulss	(%rax), %xmm1
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L185
.L184:
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L189:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm1
	jmp	.L182
.LFE42:
	.size	unroll3aa_combine, .-unroll3aa_combine
	.p2align 4,,15
.globl combine7
	.type	combine7, @function
combine7:
.LFB41:
	pushq	%r13
.LCFI58:
	movq	%rsi, %r13
	pushq	%r12
.LCFI59:
	pushq	%rbp
.LCFI60:
	pushq	%rbx
.LCFI61:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI62:
	call	vec_length
	leaq	-1(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L199
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm1
	.p2align 4,,10
	.p2align 3
.L193:
	movss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	addq	$2, %rax
	cmpq	%rax, %r12
	mulss	%xmm0, %xmm1
	jg	.L193
	leaq	-2(%rbp), %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L192:
	cmpq	%rdx, %rbp
	jle	.L194
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L195:
	addq	$1, %rdx
	mulss	(%rax), %xmm1
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L195
.L194:
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L199:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm1
	jmp	.L192
.LFE41:
	.size	combine7, .-combine7
	.p2align 4,,15
.globl unroll8x8_combine
	.type	unroll8x8_combine, @function
unroll8x8_combine:
.LFB40:
	pushq	%r12
.LCFI63:
	pushq	%rbp
.LCFI64:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI65:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	leaq	-28(%rax,%r12,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L210
	movss	.LC2(%rip), %xmm2
	movaps	%xmm2, %xmm6
	movaps	%xmm2, %xmm5
	movaps	%xmm2, %xmm0
	movaps	%xmm2, %xmm4
	movaps	%xmm2, %xmm3
	.p2align 4,,10
	.p2align 3
.L203:
	movss	20(%rax), %xmm1
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm4
	mulss	8(%rax), %xmm3
	mulss	12(%rax), %xmm5
	mulss	16(%rax), %xmm2
	mulss	24(%rax), %xmm1
	mulss	28(%rax), %xmm6
	addq	$32, %rax
	cmpq	%rax, %rcx
	ja	.L203
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-32, %rax
	leaq	32(%rdx,%rax), %rdx
.L202:
	leaq	28(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L204
	.p2align 4,,10
	.p2align 3
.L207:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L207
.L204:
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm2, %xmm0
	mulss	%xmm0, %xmm1
	mulss	%xmm6, %xmm1
	movss	%xmm1, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L210:
	movss	.LC2(%rip), %xmm2
	movaps	%xmm2, %xmm1
	movaps	%xmm2, %xmm6
	movaps	%xmm2, %xmm5
	movaps	%xmm2, %xmm4
	movaps	%xmm2, %xmm3
	movaps	%xmm2, %xmm0
	jmp	.L202
.LFE40:
	.size	unroll8x8_combine, .-unroll8x8_combine
	.p2align 4,,15
.globl unroll8x4_combine
	.type	unroll8x4_combine, @function
unroll8x4_combine:
.LFB39:
	pushq	%r12
.LCFI66:
	pushq	%rbp
.LCFI67:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI68:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	leaq	-28(%rax,%r12,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L221
	movss	.LC2(%rip), %xmm0
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L214:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm1
	mulss	8(%rax), %xmm3
	mulss	12(%rax), %xmm2
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm1
	mulss	24(%rax), %xmm3
	mulss	28(%rax), %xmm2
	addq	$32, %rax
	cmpq	%rax, %rcx
	ja	.L214
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-32, %rax
	leaq	32(%rdx,%rax), %rdx
.L213:
	leaq	28(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L215
	.p2align 4,,10
	.p2align 3
.L218:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L218
.L215:
	mulss	%xmm1, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L221:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L213
.LFE39:
	.size	unroll8x4_combine, .-unroll8x4_combine
	.p2align 4,,15
.globl unroll9x3_combine
	.type	unroll9x3_combine, @function
unroll9x3_combine:
.LFB38:
	pushq	%r12
.LCFI69:
	pushq	%rbp
.LCFI70:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI71:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	leaq	-32(%rax,%r12,4), %rsi
	movq	%rax, %rcx
	cmpq	%rsi, %rax
	jae	.L232
	movss	.LC2(%rip), %xmm0
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L225:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm2
	mulss	8(%rax), %xmm1
	mulss	12(%rax), %xmm0
	mulss	16(%rax), %xmm2
	mulss	20(%rax), %xmm1
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm2
	mulss	32(%rax), %xmm1
	addq	$36, %rax
	cmpq	%rax, %rsi
	ja	.L225
	movq	%rcx, %rdx
	movabsq	$-2049638230412172401, %rax
	notq	%rdx
	addq	%rsi, %rdx
	mulq	%rdx
	shrq	$5, %rdx
	leaq	9(%rdx,%rdx,8), %rdx
	leaq	(%rcx,%rdx,4), %rcx
.L224:
	leaq	32(%rsi), %rax
	cmpq	%rcx, %rax
	jbe	.L226
	.p2align 4,,10
	.p2align 3
.L229:
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpq	%rcx, %rax
	ja	.L229
.L226:
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L232:
	movss	.LC2(%rip), %xmm2
	movaps	%xmm2, %xmm1
	movaps	%xmm2, %xmm0
	jmp	.L224
.LFE38:
	.size	unroll9x3_combine, .-unroll9x3_combine
	.p2align 4,,15
.globl unroll8x2_combine
	.type	unroll8x2_combine, @function
unroll8x2_combine:
.LFB37:
	pushq	%r12
.LCFI72:
	pushq	%rbp
.LCFI73:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI74:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	leaq	-28(%rax,%r12,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L243
	movss	.LC2(%rip), %xmm0
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L236:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm1
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm1
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm1
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm1
	addq	$32, %rax
	cmpq	%rax, %rcx
	ja	.L236
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-32, %rax
	leaq	32(%rdx,%rax), %rdx
.L235:
	leaq	28(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L237
	.p2align 4,,10
	.p2align 3
.L240:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L240
.L237:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L243:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm0
	jmp	.L235
.LFE37:
	.size	unroll8x2_combine, .-unroll8x2_combine
	.p2align 4,,15
.globl unroll4x2as_combine
	.type	unroll4x2as_combine, @function
unroll4x2as_combine:
.LFB36:
	pushq	%r13
.LCFI75:
	movq	%rsi, %r13
	pushq	%r12
.LCFI76:
	pushq	%rbp
.LCFI77:
	pushq	%rbx
.LCFI78:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI79:
	call	vec_length
	movq	%rax, %r12
	shrq	$63, %rax
	movq	%rbx, %rdi
	leaq	(%rax,%r12), %rbp
	call	get_vec_start
	sarq	%rbp
	testq	%rbp, %rbp
	leaq	(%rax,%rbp,4), %rcx
	jle	.L253
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm0
	.p2align 4,,10
	.p2align 3
.L247:
	mulss	(%rax,%rdx,4), %xmm1
	mulss	(%rcx,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rdx, %rbp
	jg	.L247
.L246:
	leaq	(%rbp,%rbp), %rdx
	cmpq	%rdx, %r12
	jle	.L248
	leaq	(%rax,%rbp,8), %rax
	.p2align 4,,10
	.p2align 3
.L249:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L249
.L248:
	mulss	%xmm0, %xmm1
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L253:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm0
	jmp	.L246
.LFE36:
	.size	unroll4x2as_combine, .-unroll4x2as_combine
	.p2align 4,,15
.globl unrollx2as_combine
	.type	unrollx2as_combine, @function
unrollx2as_combine:
.LFB35:
	pushq	%r13
.LCFI80:
	movq	%rsi, %r13
	pushq	%r12
.LCFI81:
	pushq	%rbp
.LCFI82:
	pushq	%rbx
.LCFI83:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI84:
	call	vec_length
	movq	%rax, %r12
	shrq	$63, %rax
	movq	%rbx, %rdi
	leaq	(%rax,%r12), %rbp
	call	get_vec_start
	sarq	%rbp
	testq	%rbp, %rbp
	leaq	(%rax,%rbp,4), %rcx
	jle	.L263
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm0
	.p2align 4,,10
	.p2align 3
.L257:
	mulss	(%rax,%rdx,4), %xmm1
	mulss	(%rcx,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rdx, %rbp
	jg	.L257
.L256:
	leaq	(%rbp,%rbp), %rdx
	cmpq	%rdx, %r12
	jle	.L258
	leaq	(%rax,%rbp,8), %rax
	.p2align 4,,10
	.p2align 3
.L259:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %r12
	jg	.L259
.L258:
	mulss	%xmm0, %xmm1
	movss	%xmm1, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L263:
	movss	.LC2(%rip), %xmm1
	movaps	%xmm1, %xmm0
	jmp	.L256
.LFE35:
	.size	unrollx2as_combine, .-unrollx2as_combine
	.p2align 4,,15
.globl unroll10x10a_combine
	.type	unroll10x10a_combine, @function
unroll10x10a_combine:
.LFB34:
	pushq	%r12
.LCFI85:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI86:
	pushq	%rbx
.LCFI87:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	cmpq	$9, %rbp
	movq	%rax, %rdi
	jle	.L273
	leaq	-10(%rbp), %rdx
	movq	%rax, %rcx
	movabsq	$-3689348814741910323, %rax
	movss	.LC2(%rip), %xmm0
	xorl	%esi, %esi
	mulq	%rdx
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm9
	movaps	%xmm0, %xmm8
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm6
	shrq	$3, %rdx
	movaps	%xmm0, %xmm5
	leaq	5(%rdx,%rdx,4), %rdx
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	leaq	0(,%rdx,8), %rax
	.p2align 4,,10
	.p2align 3
.L267:
	addq	$40, %rsi
	mulss	(%rcx), %xmm0
	mulss	4(%rcx), %xmm1
	mulss	8(%rcx), %xmm9
	mulss	12(%rcx), %xmm8
	mulss	16(%rcx), %xmm7
	mulss	20(%rcx), %xmm6
	mulss	24(%rcx), %xmm5
	mulss	28(%rcx), %xmm4
	mulss	32(%rcx), %xmm3
	mulss	36(%rcx), %xmm2
	addq	$40, %rcx
	cmpq	%rax, %rsi
	jne	.L267
	addq	%rdx, %rdx
.L266:
	cmpq	%rdx, %rbp
	jle	.L268
	leaq	(%rdi,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L269:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L269
.L268:
	mulss	%xmm1, %xmm0
	popq	%rbx
	popq	%rbp
	mulss	%xmm9, %xmm0
	mulss	%xmm8, %xmm0
	mulss	%xmm7, %xmm0
	mulss	%xmm6, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r12)
	popq	%r12
	ret
.L273:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm9
	movaps	%xmm1, %xmm8
	movaps	%xmm1, %xmm7
	movaps	%xmm1, %xmm6
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L266
.LFE34:
	.size	unroll10x10a_combine, .-unroll10x10a_combine
	.p2align 4,,15
.globl unroll8x8a_combine
	.type	unroll8x8a_combine, @function
unroll8x8a_combine:
.LFB33:
	pushq	%r13
.LCFI88:
	movq	%rsi, %r13
	pushq	%r12
.LCFI89:
	pushq	%rbp
.LCFI90:
	pushq	%rbx
.LCFI91:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI92:
	call	vec_length
	leaq	-7(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rdx
	jle	.L283
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm6
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L277:
	mulss	(%rdx,%rax,4), %xmm0
	mulss	4(%rdx,%rax,4), %xmm1
	mulss	8(%rdx,%rax,4), %xmm7
	mulss	12(%rdx,%rax,4), %xmm6
	mulss	16(%rdx,%rax,4), %xmm5
	mulss	20(%rdx,%rax,4), %xmm4
	mulss	24(%rdx,%rax,4), %xmm3
	mulss	28(%rdx,%rax,4), %xmm2
	addq	$8, %rax
	cmpq	%rax, %r12
	jg	.L277
	leaq	-8(%rbp), %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rcx
.L276:
	cmpq	%rcx, %rbp
	jle	.L278
	leaq	(%rdx,%rcx,4), %rax
	.p2align 4,,10
	.p2align 3
.L279:
	addq	$1, %rcx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rcx, %rbp
	jg	.L279
.L278:
	mulss	%xmm1, %xmm0
	mulss	%xmm7, %xmm0
	mulss	%xmm6, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L283:
	movss	.LC2(%rip), %xmm1
	xorl	%ecx, %ecx
	movaps	%xmm1, %xmm7
	movaps	%xmm1, %xmm6
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L276
.LFE33:
	.size	unroll8x8a_combine, .-unroll8x8a_combine
	.p2align 4,,15
.globl unroll6x6a_combine
	.type	unroll6x6a_combine, @function
unroll6x6a_combine:
.LFB32:
	pushq	%r13
.LCFI93:
	movq	%rsi, %r13
	pushq	%r12
.LCFI94:
	pushq	%rbp
.LCFI95:
	pushq	%rbx
.LCFI96:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI97:
	call	vec_length
	leaq	-5(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L293
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm4
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L287:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm1
	mulss	8(%rcx,%rax,4), %xmm5
	mulss	12(%rcx,%rax,4), %xmm4
	mulss	16(%rcx,%rax,4), %xmm3
	mulss	20(%rcx,%rax,4), %xmm2
	addq	$6, %rax
	cmpq	%rax, %r12
	jg	.L287
	leaq	-6(%rbp), %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	addq	%rdx, %rdx
.L286:
	cmpq	%rdx, %rbp
	jle	.L288
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L289:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L289
.L288:
	mulss	%xmm1, %xmm0
	mulss	%xmm5, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L293:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L286
.LFE32:
	.size	unroll6x6a_combine, .-unroll6x6a_combine
	.p2align 4,,15
.globl unroll5x5a_combine
	.type	unroll5x5a_combine, @function
unroll5x5a_combine:
.LFB31:
	pushq	%r13
.LCFI98:
	movq	%rsi, %r13
	pushq	%r12
.LCFI99:
	pushq	%rbp
.LCFI100:
	pushq	%rbx
.LCFI101:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI102:
	call	vec_length
	leaq	-4(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L303
	movss	.LC2(%rip), %xmm1
	xorl	%eax, %eax
	movaps	%xmm1, %xmm0
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	.p2align 4,,10
	.p2align 3
.L297:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm4
	mulss	8(%rcx,%rax,4), %xmm3
	mulss	12(%rcx,%rax,4), %xmm2
	mulss	16(%rcx,%rax,4), %xmm1
	addq	$5, %rax
	cmpq	%rax, %r12
	jg	.L297
	leaq	-5(%rbp), %rdx
	movabsq	$-3689348814741910323, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	5(%rdx,%rdx,4), %rdx
.L296:
	cmpq	%rdx, %rbp
	jle	.L298
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L299:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L299
.L298:
	mulss	%xmm4, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L303:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L296
.LFE31:
	.size	unroll5x5a_combine, .-unroll5x5a_combine
	.p2align 4,,15
.globl unroll12x12a_combine
	.type	unroll12x12a_combine, @function
unroll12x12a_combine:
.LFB30:
	pushq	%r12
.LCFI103:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI104:
	pushq	%rbx
.LCFI105:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	cmpq	$11, %rbp
	movq	%rax, %rdi
	jle	.L313
	leaq	-12(%rbp), %rdx
	movq	%rax, %rcx
	movabsq	$-6148914691236517205, %rax
	movss	.LC2(%rip), %xmm0
	xorl	%esi, %esi
	mulq	%rdx
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm10
	movaps	%xmm0, %xmm11
	movaps	%xmm0, %xmm8
	movaps	%xmm0, %xmm9
	shrq	$3, %rdx
	movaps	%xmm0, %xmm6
	leaq	3(%rdx,%rdx,2), %rdx
	movaps	%xmm0, %xmm7
	movaps	%xmm0, %xmm4
	movq	%rdx, %rax
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm2
	salq	$4, %rax
	movaps	%xmm0, %xmm3
	.p2align 4,,10
	.p2align 3
.L307:
	addq	$48, %rsi
	mulss	(%rcx), %xmm0
	mulss	24(%rcx), %xmm6
	mulss	4(%rcx), %xmm1
	mulss	28(%rcx), %xmm7
	mulss	8(%rcx), %xmm10
	mulss	32(%rcx), %xmm4
	mulss	12(%rcx), %xmm11
	mulss	36(%rcx), %xmm5
	mulss	16(%rcx), %xmm8
	mulss	40(%rcx), %xmm2
	mulss	20(%rcx), %xmm9
	mulss	44(%rcx), %xmm3
	addq	$48, %rcx
	cmpq	%rax, %rsi
	jne	.L307
	salq	$2, %rdx
.L306:
	cmpq	%rdx, %rbp
	jle	.L308
	leaq	(%rdi,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L309:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L309
.L308:
	mulss	%xmm1, %xmm0
	mulss	%xmm11, %xmm10
	mulss	%xmm9, %xmm8
	mulss	%xmm7, %xmm6
	mulss	%xmm5, %xmm4
	mulss	%xmm10, %xmm0
	mulss	%xmm3, %xmm2
	popq	%rbx
	popq	%rbp
	mulss	%xmm8, %xmm0
	mulss	%xmm6, %xmm0
	mulss	%xmm4, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r12)
	popq	%r12
	ret
.L313:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm10
	movaps	%xmm1, %xmm11
	movaps	%xmm1, %xmm8
	movaps	%xmm1, %xmm9
	movaps	%xmm1, %xmm6
	movaps	%xmm1, %xmm7
	movaps	%xmm1, %xmm4
	movaps	%xmm1, %xmm5
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm0
	jmp	.L306
.LFE30:
	.size	unroll12x12a_combine, .-unroll12x12a_combine
	.p2align 4,,15
.globl unroll12x6a_combine
	.type	unroll12x6a_combine, @function
unroll12x6a_combine:
.LFB29:
	pushq	%r13
.LCFI106:
	movq	%rsi, %r13
	pushq	%r12
.LCFI107:
	pushq	%rbp
.LCFI108:
	pushq	%rbx
.LCFI109:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI110:
	call	vec_length
	leaq	-11(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L323
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm5
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm4
	.p2align 4,,10
	.p2align 3
.L317:
	mulss	(%rax), %xmm0
	addq	$12, %rdx
	mulss	4(%rax), %xmm3
	mulss	8(%rax), %xmm2
	mulss	12(%rax), %xmm5
	mulss	16(%rax), %xmm1
	mulss	20(%rax), %xmm4
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm3
	mulss	32(%rax), %xmm2
	mulss	36(%rax), %xmm5
	mulss	40(%rax), %xmm1
	mulss	44(%rax), %xmm4
	addq	$48, %rax
	cmpq	%rdx, %r12
	jg	.L317
	leaq	-12(%rbp), %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$3, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	salq	$2, %rdx
.L316:
	cmpq	%rdx, %rbp
	jle	.L318
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L319:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L319
.L318:
	mulss	%xmm3, %xmm0
	mulss	%xmm5, %xmm2
	mulss	%xmm4, %xmm1
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L323:
	movss	.LC2(%rip), %xmm3
	xorl	%edx, %edx
	movaps	%xmm3, %xmm2
	movaps	%xmm3, %xmm5
	movaps	%xmm3, %xmm1
	movaps	%xmm3, %xmm4
	movaps	%xmm3, %xmm0
	jmp	.L316
.LFE29:
	.size	unroll12x6a_combine, .-unroll12x6a_combine
	.p2align 4,,15
.globl unroll8x4a_combine
	.type	unroll8x4a_combine, @function
unroll8x4a_combine:
.LFB28:
	pushq	%r13
.LCFI111:
	movq	%rsi, %r13
	pushq	%r12
.LCFI112:
	pushq	%rbp
.LCFI113:
	pushq	%rbx
.LCFI114:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI115:
	call	vec_length
	leaq	-7(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rdx
	jle	.L333
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L327:
	mulss	(%rdx,%rax,4), %xmm0
	mulss	4(%rdx,%rax,4), %xmm1
	mulss	8(%rdx,%rax,4), %xmm3
	mulss	12(%rdx,%rax,4), %xmm2
	mulss	16(%rdx,%rax,4), %xmm0
	mulss	20(%rdx,%rax,4), %xmm1
	mulss	24(%rdx,%rax,4), %xmm3
	mulss	28(%rdx,%rax,4), %xmm2
	addq	$8, %rax
	cmpq	%rax, %r12
	jg	.L327
	leaq	-8(%rbp), %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rcx
.L326:
	cmpq	%rcx, %rbp
	jle	.L328
	leaq	(%rdx,%rcx,4), %rax
	.p2align 4,,10
	.p2align 3
.L329:
	addq	$1, %rcx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rcx, %rbp
	jg	.L329
.L328:
	mulss	%xmm1, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L333:
	movss	.LC2(%rip), %xmm1
	xorl	%ecx, %ecx
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L326
.LFE28:
	.size	unroll8x4a_combine, .-unroll8x4a_combine
	.p2align 4,,15
.globl unroll4x4a_combine
	.type	unroll4x4a_combine, @function
unroll4x4a_combine:
.LFB27:
	pushq	%r13
.LCFI116:
	movq	%rsi, %r13
	pushq	%r12
.LCFI117:
	pushq	%rbp
.LCFI118:
	pushq	%rbx
.LCFI119:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI120:
	call	vec_length
	leaq	-3(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L343
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	movaps	%xmm0, %xmm3
	movaps	%xmm0, %xmm2
	.p2align 4,,10
	.p2align 3
.L337:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm1
	mulss	8(%rcx,%rax,4), %xmm3
	mulss	12(%rcx,%rax,4), %xmm2
	addq	$4, %rax
	cmpq	%rax, %r12
	jg	.L337
	leaq	-4(%rbp), %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L336:
	cmpq	%rdx, %rbp
	jle	.L338
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L339:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L339
.L338:
	mulss	%xmm1, %xmm0
	mulss	%xmm3, %xmm0
	mulss	%xmm2, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L343:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm3
	movaps	%xmm1, %xmm2
	movaps	%xmm1, %xmm0
	jmp	.L336
.LFE27:
	.size	unroll4x4a_combine, .-unroll4x4a_combine
	.p2align 4,,15
.globl unroll3x3a_combine
	.type	unroll3x3a_combine, @function
unroll3x3a_combine:
.LFB26:
	pushq	%r13
.LCFI121:
	movq	%rsi, %r13
	pushq	%r12
.LCFI122:
	pushq	%rbp
.LCFI123:
	pushq	%rbx
.LCFI124:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI125:
	call	vec_length
	leaq	-2(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L353
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm2
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L347:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm2
	mulss	8(%rcx,%rax,4), %xmm1
	addq	$3, %rax
	cmpq	%rax, %r12
	jg	.L347
	leaq	-3(%rbp), %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	%rdx
	leaq	3(%rdx,%rdx,2), %rdx
.L346:
	cmpq	%rdx, %rbp
	jle	.L348
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L349:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L349
.L348:
	mulss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L353:
	movss	.LC2(%rip), %xmm2
	xorl	%edx, %edx
	movaps	%xmm2, %xmm1
	movaps	%xmm2, %xmm0
	jmp	.L346
.LFE26:
	.size	unroll3x3a_combine, .-unroll3x3a_combine
	.p2align 4,,15
.globl unroll8x2a_combine
	.type	unroll8x2a_combine, @function
unroll8x2a_combine:
.LFB25:
	pushq	%r13
.LCFI126:
	movq	%rsi, %r13
	pushq	%r12
.LCFI127:
	pushq	%rbp
.LCFI128:
	pushq	%rbx
.LCFI129:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI130:
	call	vec_length
	leaq	-7(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rdx
	jle	.L363
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L357:
	mulss	(%rdx,%rax,4), %xmm0
	mulss	4(%rdx,%rax,4), %xmm1
	mulss	8(%rdx,%rax,4), %xmm0
	mulss	12(%rdx,%rax,4), %xmm1
	mulss	16(%rdx,%rax,4), %xmm0
	mulss	20(%rdx,%rax,4), %xmm1
	mulss	24(%rdx,%rax,4), %xmm0
	mulss	28(%rdx,%rax,4), %xmm1
	addq	$8, %rax
	cmpq	%rax, %r12
	jg	.L357
	leaq	-8(%rbp), %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rcx
.L356:
	cmpq	%rcx, %rbp
	jle	.L358
	leaq	(%rdx,%rcx,4), %rax
	.p2align 4,,10
	.p2align 3
.L359:
	addq	$1, %rcx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rcx, %rbp
	jg	.L359
.L358:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L363:
	movss	.LC2(%rip), %xmm1
	xorl	%ecx, %ecx
	movaps	%xmm1, %xmm0
	jmp	.L356
.LFE25:
	.size	unroll8x2a_combine, .-unroll8x2a_combine
	.p2align 4,,15
.globl unroll4x2a_combine
	.type	unroll4x2a_combine, @function
unroll4x2a_combine:
.LFB24:
	pushq	%r13
.LCFI131:
	movq	%rsi, %r13
	pushq	%r12
.LCFI132:
	pushq	%rbp
.LCFI133:
	pushq	%rbx
.LCFI134:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI135:
	call	vec_length
	leaq	-3(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L373
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	movaps	%xmm0, %xmm1
	.p2align 4,,10
	.p2align 3
.L367:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm1
	mulss	8(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm1
	addq	$4, %rax
	cmpq	%rax, %r12
	jg	.L367
	leaq	-4(%rbp), %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L366:
	cmpq	%rdx, %rbp
	jle	.L368
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L369:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L369
.L368:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L373:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm0
	jmp	.L366
.LFE24:
	.size	unroll4x2a_combine, .-unroll4x2a_combine
	.p2align 4,,15
.globl combine6
	.type	combine6, @function
combine6:
.LFB23:
	pushq	%r13
.LCFI136:
	movq	%rsi, %r13
	pushq	%r12
.LCFI137:
	pushq	%rbp
.LCFI138:
	pushq	%rbx
.LCFI139:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI140:
	call	vec_length
	leaq	-1(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L383
	movss	.LC2(%rip), %xmm1
	xorl	%eax, %eax
	movaps	%xmm1, %xmm0
	.p2align 4,,10
	.p2align 3
.L377:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm1
	addq	$2, %rax
	cmpq	%rax, %r12
	jg	.L377
	leaq	-2(%rbp), %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L376:
	cmpq	%rdx, %rbp
	jle	.L378
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L379:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L379
.L378:
	mulss	%xmm1, %xmm0
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L383:
	movss	.LC2(%rip), %xmm1
	xorl	%edx, %edx
	movaps	%xmm1, %xmm0
	jmp	.L376
.LFE23:
	.size	combine6, .-combine6
	.p2align 4,,15
.globl unroll16_combine
	.type	unroll16_combine, @function
unroll16_combine:
.LFB22:
	pushq	%r12
.LCFI141:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI142:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI143:
	call	vec_length
	movq	%r12, %rdi
	movq	%rax, %rbx
	call	get_vec_start
	movq	%rax, %rdx
	movq	%rbx, %rax
	sarq	$63, %rax
	shrq	$60, %rax
	leaq	(%rbx,%rax), %rsi
	andl	$15, %esi
	subq	%rax, %rsi
	subq	%rsi, %rbx
	leaq	(%rdx,%rbx,4), %rcx
	cmpq	%rcx, %rdx
	jae	.L394
	movq	%rdx, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L387:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm0
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm0
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm0
	mulss	32(%rax), %xmm0
	mulss	36(%rax), %xmm0
	mulss	40(%rax), %xmm0
	mulss	44(%rax), %xmm0
	mulss	48(%rax), %xmm0
	mulss	52(%rax), %xmm0
	mulss	56(%rax), %xmm0
	mulss	60(%rax), %xmm0
	addq	$64, %rax
	cmpq	%rax, %rcx
	ja	.L387
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-64, %rax
	leaq	64(%rdx,%rax), %rdx
.L386:
	leaq	(%rcx,%rsi,4), %rax
	cmpq	%rdx, %rax
	jbe	.L388
	.p2align 4,,10
	.p2align 3
.L391:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L391
.L388:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L394:
	movss	.LC2(%rip), %xmm0
	jmp	.L386
.LFE22:
	.size	unroll16_combine, .-unroll16_combine
	.p2align 4,,15
.globl unroll8_combine
	.type	unroll8_combine, @function
unroll8_combine:
.LFB21:
	pushq	%r12
.LCFI144:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI145:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI146:
	call	vec_length
	movq	%r12, %rdi
	movq	%rax, %rbx
	call	get_vec_start
	movq	%rax, %rdx
	movq	%rbx, %rax
	sarq	$63, %rax
	shrq	$61, %rax
	leaq	(%rbx,%rax), %rsi
	andl	$7, %esi
	subq	%rax, %rsi
	subq	%rsi, %rbx
	leaq	(%rdx,%rbx,4), %rcx
	cmpq	%rcx, %rdx
	jae	.L405
	movq	%rdx, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L398:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm0
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm0
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm0
	addq	$32, %rax
	cmpq	%rax, %rcx
	ja	.L398
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-32, %rax
	leaq	32(%rdx,%rax), %rdx
.L397:
	leaq	(%rcx,%rsi,4), %rax
	cmpq	%rdx, %rax
	jbe	.L399
	.p2align 4,,10
	.p2align 3
.L402:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L402
.L399:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L405:
	movss	.LC2(%rip), %xmm0
	jmp	.L397
.LFE21:
	.size	unroll8_combine, .-unroll8_combine
	.p2align 4,,15
.globl unroll4_combine
	.type	unroll4_combine, @function
unroll4_combine:
.LFB20:
	pushq	%r12
.LCFI147:
	pushq	%rbp
.LCFI148:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI149:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	leaq	-12(%rax,%r12,4), %rcx
	movq	%rax, %rdx
	cmpq	%rcx, %rax
	jae	.L416
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L409:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm0
	addq	$16, %rax
	cmpq	%rax, %rcx
	ja	.L409
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	andq	$-16, %rax
	leaq	16(%rdx,%rax), %rdx
.L408:
	leaq	12(%rcx), %rax
	cmpq	%rdx, %rax
	jbe	.L410
	.p2align 4,,10
	.p2align 3
.L413:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L413
.L410:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L416:
	movss	.LC2(%rip), %xmm0
	jmp	.L408
.LFE20:
	.size	unroll4_combine, .-unroll4_combine
	.p2align 4,,15
.globl unroll3_combine
	.type	unroll3_combine, @function
unroll3_combine:
.LFB19:
	pushq	%r12
.LCFI150:
	pushq	%rbp
.LCFI151:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI152:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	leaq	-8(%rax,%r12,4), %rsi
	movq	%rax, %rcx
	cmpq	%rsi, %rax
	jae	.L427
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L420:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	addq	$12, %rax
	cmpq	%rax, %rsi
	ja	.L420
	movq	%rcx, %rdx
	movabsq	$-6148914691236517205, %rax
	notq	%rdx
	addq	%rsi, %rdx
	mulq	%rdx
	shrq	$3, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	leaq	(%rcx,%rdx,4), %rcx
.L419:
	leaq	8(%rsi), %rax
	cmpq	%rcx, %rax
	jbe	.L421
	.p2align 4,,10
	.p2align 3
.L424:
	mulss	(%rcx), %xmm0
	addq	$4, %rcx
	cmpq	%rcx, %rax
	ja	.L424
.L421:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L427:
	movss	.LC2(%rip), %xmm0
	jmp	.L419
.LFE19:
	.size	unroll3_combine, .-unroll3_combine
	.p2align 4,,15
.globl unroll2_combine
	.type	unroll2_combine, @function
unroll2_combine:
.LFB18:
	pushq	%r12
.LCFI153:
	pushq	%rbp
.LCFI154:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI155:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	movq	%rax, %rdx
	movq	%r12, %rax
	shrq	$63, %rax
	leaq	(%r12,%rax), %rsi
	andl	$1, %esi
	subq	%rax, %rsi
	subq	%rsi, %r12
	leaq	(%rdx,%r12,4), %rcx
	cmpq	%rcx, %rdx
	jae	.L438
	movq	%rdx, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L431:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	addq	$8, %rax
	cmpq	%rax, %rcx
	ja	.L431
	movq	%rdx, %rax
	notq	%rax
	addq	%rcx, %rax
	shrq	$3, %rax
	leaq	8(%rdx,%rax,8), %rdx
.L430:
	leaq	(%rcx,%rsi,4), %rax
	cmpq	%rdx, %rax
	jbe	.L432
	.p2align 4,,10
	.p2align 3
.L435:
	mulss	(%rdx), %xmm0
	addq	$4, %rdx
	cmpq	%rdx, %rax
	ja	.L435
.L432:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L438:
	movss	.LC2(%rip), %xmm0
	jmp	.L430
.LFE18:
	.size	unroll2_combine, .-unroll2_combine
	.p2align 4,,15
.globl unroll16a_combine
	.type	unroll16a_combine, @function
unroll16a_combine:
.LFB17:
	pushq	%r13
.LCFI156:
	movq	%rsi, %r13
	pushq	%r12
.LCFI157:
	pushq	%rbp
.LCFI158:
	pushq	%rbx
.LCFI159:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI160:
	call	vec_length
	leaq	-15(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L448
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L442:
	mulss	(%rax), %xmm0
	addq	$16, %rdx
	mulss	4(%rax), %xmm0
	mulss	8(%rax), %xmm0
	mulss	12(%rax), %xmm0
	mulss	16(%rax), %xmm0
	mulss	20(%rax), %xmm0
	mulss	24(%rax), %xmm0
	mulss	28(%rax), %xmm0
	mulss	32(%rax), %xmm0
	mulss	36(%rax), %xmm0
	mulss	40(%rax), %xmm0
	mulss	44(%rax), %xmm0
	mulss	48(%rax), %xmm0
	mulss	52(%rax), %xmm0
	mulss	56(%rax), %xmm0
	mulss	60(%rax), %xmm0
	addq	$64, %rax
	cmpq	%rdx, %r12
	jg	.L442
	leaq	-16(%rbp), %rax
	andq	$-16, %rax
	leaq	16(%rax), %rdx
.L441:
	cmpq	%rdx, %rbp
	jle	.L443
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L444:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L444
.L443:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L448:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L441
.LFE17:
	.size	unroll16a_combine, .-unroll16a_combine
	.p2align 4,,15
.globl unroll8a_combine
	.type	unroll8a_combine, @function
unroll8a_combine:
.LFB16:
	pushq	%r13
.LCFI161:
	movq	%rsi, %r13
	pushq	%r12
.LCFI162:
	pushq	%rbp
.LCFI163:
	pushq	%rbx
.LCFI164:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI165:
	call	vec_length
	leaq	-7(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rdx
	jle	.L458
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L452:
	mulss	(%rdx,%rax,4), %xmm0
	mulss	4(%rdx,%rax,4), %xmm0
	mulss	8(%rdx,%rax,4), %xmm0
	mulss	12(%rdx,%rax,4), %xmm0
	mulss	16(%rdx,%rax,4), %xmm0
	mulss	20(%rdx,%rax,4), %xmm0
	mulss	24(%rdx,%rax,4), %xmm0
	mulss	28(%rdx,%rax,4), %xmm0
	addq	$8, %rax
	cmpq	%rax, %r12
	jg	.L452
	leaq	-8(%rbp), %rax
	shrq	$3, %rax
	leaq	8(,%rax,8), %rcx
.L451:
	cmpq	%rcx, %rbp
	jle	.L453
	leaq	(%rdx,%rcx,4), %rax
	.p2align 4,,10
	.p2align 3
.L454:
	addq	$1, %rcx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rcx, %rbp
	jg	.L454
.L453:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L458:
	xorl	%ecx, %ecx
	movss	.LC2(%rip), %xmm0
	jmp	.L451
.LFE16:
	.size	unroll8a_combine, .-unroll8a_combine
	.p2align 4,,15
.globl unroll6a_combine
	.type	unroll6a_combine, @function
unroll6a_combine:
.LFB15:
	pushq	%r13
.LCFI166:
	movq	%rsi, %r13
	pushq	%r12
.LCFI167:
	pushq	%rbp
.LCFI168:
	pushq	%rbx
.LCFI169:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI170:
	call	vec_length
	leaq	-5(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L468
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L462:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm0
	mulss	16(%rcx,%rax,4), %xmm0
	mulss	20(%rcx,%rax,4), %xmm0
	addq	$6, %rax
	cmpq	%rax, %r12
	jg	.L462
	leaq	-6(%rbp), %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	3(%rdx,%rdx,2), %rdx
	addq	%rdx, %rdx
.L461:
	cmpq	%rdx, %rbp
	jle	.L463
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L464:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L464
.L463:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L468:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L461
.LFE15:
	.size	unroll6a_combine, .-unroll6a_combine
	.p2align 4,,15
.globl unroll5a_combine
	.type	unroll5a_combine, @function
unroll5a_combine:
.LFB14:
	pushq	%r13
.LCFI171:
	movq	%rsi, %r13
	pushq	%r12
.LCFI172:
	pushq	%rbp
.LCFI173:
	pushq	%rbx
.LCFI174:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI175:
	call	vec_length
	leaq	-4(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L478
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L472:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm0
	mulss	16(%rcx,%rax,4), %xmm0
	addq	$5, %rax
	cmpq	%rax, %r12
	jg	.L472
	leaq	-5(%rbp), %rdx
	movabsq	$-3689348814741910323, %rax
	mulq	%rdx
	shrq	$2, %rdx
	leaq	5(%rdx,%rdx,4), %rdx
.L471:
	cmpq	%rdx, %rbp
	jle	.L473
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L474:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L474
.L473:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L478:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L471
.LFE14:
	.size	unroll5a_combine, .-unroll5a_combine
	.p2align 4,,15
.globl unroll4a_combine
	.type	unroll4a_combine, @function
unroll4a_combine:
.LFB13:
	pushq	%r13
.LCFI176:
	movq	%rsi, %r13
	pushq	%r12
.LCFI177:
	pushq	%rbp
.LCFI178:
	pushq	%rbx
.LCFI179:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI180:
	call	vec_length
	leaq	-3(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L488
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L482:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	mulss	12(%rcx,%rax,4), %xmm0
	addq	$4, %rax
	cmpq	%rax, %r12
	jg	.L482
	leaq	-4(%rbp), %rax
	shrq	$2, %rax
	leaq	4(,%rax,4), %rdx
.L481:
	cmpq	%rdx, %rbp
	jle	.L483
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L484:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L484
.L483:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L488:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L481
.LFE13:
	.size	unroll4a_combine, .-unroll4a_combine
	.p2align 4,,15
.globl unroll2aw_combine
	.type	unroll2aw_combine, @function
unroll2aw_combine:
.LFB12:
	pushq	%r13
.LCFI181:
	movq	%rsi, %r13
	pushq	%r12
.LCFI182:
	pushq	%rbp
.LCFI183:
	pushq	%rbx
.LCFI184:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI185:
	call	vec_length
	leaq	-1(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L498
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L492:
	mulss	(%rcx,%rax,4), %xmm0
	addq	$2, %rax
	cmpq	%rax, %r12
	mulss	-4(%rcx,%rax,4), %xmm0
	jg	.L492
	leaq	-2(%rbp), %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L491:
	cmpq	%rdx, %rbp
	jle	.L493
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L494:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L494
.L493:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L498:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L491
.LFE12:
	.size	unroll2aw_combine, .-unroll2aw_combine
	.p2align 4,,15
.globl combine5p
	.type	combine5p, @function
combine5p:
.LFB11:
	pushq	%r12
.LCFI186:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI187:
	pushq	%rbx
.LCFI188:
	movq	%rdi, %rbx
	call	get_vec_start
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	vec_length
	leaq	(%rbp,%rax,4), %rdx
	leaq	-4(%rdx), %rcx
	cmpq	%rcx, %rbp
	jae	.L510
	movq	%rbp, %rax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L502:
	mulss	(%rax), %xmm0
	mulss	4(%rax), %xmm0
	addq	$8, %rax
	cmpq	%rax, %rcx
	ja	.L502
	movq	%rdx, %rax
	subq	%rbp, %rax
	subq	$5, %rax
	shrq	$3, %rax
	leaq	8(%rbp,%rax,8), %rbp
	jmp	.L509
	.p2align 4,,10
	.p2align 3
.L506:
	mulss	(%rbp), %xmm0
	addq	$4, %rbp
.L509:
	cmpq	%rbp, %rdx
	ja	.L506
	popq	%rbx
	popq	%rbp
	movss	%xmm0, (%r12)
	popq	%r12
	ret
.L510:
	movss	.LC2(%rip), %xmm0
	jmp	.L509
.LFE11:
	.size	combine5p, .-combine5p
	.p2align 4,,15
.globl unroll3a_combine
	.type	unroll3a_combine, @function
unroll3a_combine:
.LFB10:
	pushq	%r13
.LCFI189:
	movq	%rsi, %r13
	pushq	%r12
.LCFI190:
	pushq	%rbp
.LCFI191:
	pushq	%rbx
.LCFI192:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI193:
	call	vec_length
	leaq	-2(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L520
	movss	.LC2(%rip), %xmm0
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L514:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	mulss	8(%rcx,%rax,4), %xmm0
	addq	$3, %rax
	cmpq	%rax, %r12
	jg	.L514
	leaq	-3(%rbp), %rdx
	movabsq	$-6148914691236517205, %rax
	mulq	%rdx
	shrq	%rdx
	leaq	3(%rdx,%rdx,2), %rdx
.L513:
	cmpq	%rdx, %rbp
	jle	.L515
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L516:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L516
.L515:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L520:
	movss	.LC2(%rip), %xmm0
	xorl	%edx, %edx
	jmp	.L513
.LFE10:
	.size	unroll3a_combine, .-unroll3a_combine
	.p2align 4,,15
.globl combine5
	.type	combine5, @function
combine5:
.LFB9:
	pushq	%r13
.LCFI194:
	movq	%rsi, %r13
	pushq	%r12
.LCFI195:
	pushq	%rbp
.LCFI196:
	pushq	%rbx
.LCFI197:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI198:
	call	vec_length
	leaq	-1(%rax), %r12
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%r12, %r12
	movq	%rax, %rcx
	jle	.L530
	xorl	%eax, %eax
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L524:
	mulss	(%rcx,%rax,4), %xmm0
	mulss	4(%rcx,%rax,4), %xmm0
	addq	$2, %rax
	cmpq	%rax, %r12
	jg	.L524
	leaq	-2(%rbp), %rax
	shrq	%rax
	leaq	2(%rax,%rax), %rdx
.L523:
	cmpq	%rdx, %rbp
	jle	.L525
	leaq	(%rcx,%rdx,4), %rax
	.p2align 4,,10
	.p2align 3
.L526:
	addq	$1, %rdx
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rdx, %rbp
	jg	.L526
.L525:
	movss	%xmm0, (%r13)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.L530:
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	jmp	.L523
.LFE9:
	.size	combine5, .-combine5
	.p2align 4,,15
.globl combine4p
	.type	combine4p, @function
combine4p:
.LFB8:
	pushq	%r12
.LCFI199:
	pushq	%rbp
.LCFI200:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI201:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %r12
	call	get_vec_start
	leaq	(%rax,%r12,4), %rdx
	cmpq	%rdx, %rax
	jae	.L537
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L534:
	mulss	(%rax), %xmm0
	addq	$4, %rax
	cmpq	%rax, %rdx
	ja	.L534
.L533:
	movss	%xmm0, (%rbp)
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.L537:
	movss	.LC2(%rip), %xmm0
	jmp	.L533
.LFE8:
	.size	combine4p, .-combine4p
	.p2align 4,,15
.globl combine4b
	.type	combine4b, @function
combine4b:
.LFB7:
	pushq	%rbp
.LCFI202:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI203:
	movq	%rdi, %rbx
	subq	$8, %rsp
.LCFI204:
	call	vec_length
	testq	%rax, %rax
	movq	%rax, %rcx
	jle	.L545
	movq	(%rbx), %rsi
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L542:
	cmpq	%rdx, %rsi
	jle	.L541
	movq	8(%rbx), %rax
	mulss	(%rax,%rdx,4), %xmm0
.L541:
	addq	$1, %rdx
	cmpq	%rdx, %rcx
	jg	.L542
.L540:
	movss	%xmm0, (%rbp)
	addq	$8, %rsp
	popq	%rbx
	popq	%rbp
	ret
.L545:
	movss	.LC2(%rip), %xmm0
	jmp	.L540
.LFE7:
	.size	combine4b, .-combine4b
	.p2align 4,,15
.globl combine4
	.type	combine4, @function
combine4:
.LFB6:
	pushq	%r12
.LCFI205:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI206:
	pushq	%rbx
.LCFI207:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%rbp, %rbp
	jle	.L552
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L549:
	mulss	(%rax,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rdx, %rbp
	jg	.L549
.L548:
	popq	%rbx
	popq	%rbp
	movss	%xmm0, (%r12)
	popq	%r12
	ret
.L552:
	movss	.LC2(%rip), %xmm0
	jmp	.L548
.LFE6:
	.size	combine4, .-combine4
	.p2align 4,,15
.globl combine3w
	.type	combine3w, @function
combine3w:
.LFB5:
	pushq	%r12
.LCFI208:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI209:
	pushq	%rbx
.LCFI210:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	testq	%rbp, %rbp
	jle	.L556
	xorl	%edx, %edx
	movss	.LC2(%rip), %xmm0
	.p2align 4,,10
	.p2align 3
.L555:
	mulss	(%rax,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rdx, %rbp
	movss	%xmm0, (%r12)
	jg	.L555
.L556:
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.LFE5:
	.size	combine3w, .-combine3w
	.p2align 4,,15
.globl combine3
	.type	combine3, @function
combine3:
.LFB4:
	pushq	%r12
.LCFI211:
	movq	%rsi, %r12
	pushq	%rbp
.LCFI212:
	pushq	%rbx
.LCFI213:
	movq	%rdi, %rbx
	call	vec_length
	movq	%rbx, %rdi
	movq	%rax, %rbp
	call	get_vec_start
	movss	.LC2(%rip), %xmm0
	testq	%rbp, %rbp
	movss	%xmm0, (%r12)
	jle	.L561
	xorl	%edx, %edx
	.p2align 4,,10
	.p2align 3
.L560:
	mulss	(%rax,%rdx,4), %xmm0
	addq	$1, %rdx
	cmpq	%rdx, %rbp
	movss	%xmm0, (%r12)
	jg	.L560
.L561:
	popq	%rbx
	popq	%rbp
	popq	%r12
	ret
.LFE4:
	.size	combine3, .-combine3
	.p2align 4,,15
.globl combine2
	.type	combine2, @function
combine2:
.LFB3:
	pushq	%r14
.LCFI214:
	pushq	%r13
.LCFI215:
	movq	%rdi, %r13
	pushq	%r12
.LCFI216:
	pushq	%rbp
.LCFI217:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI218:
	subq	$16, %rsp
.LCFI219:
	call	vec_length
	testq	%rax, %rax
	movq	%rax, %r12
	movl	$0x3f800000, (%rbp)
	jle	.L566
	leaq	12(%rsp), %r14
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L565:
	movq	%rbx, %rsi
	movq	%r14, %rdx
	movq	%r13, %rdi
	call	get_vec_element
	movss	(%rbp), %xmm0
	addq	$1, %rbx
	mulss	12(%rsp), %xmm0
	cmpq	%rbx, %r12
	movss	%xmm0, (%rbp)
	jg	.L565
.L566:
	addq	$16, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
.LFE3:
	.size	combine2, .-combine2
	.p2align 4,,15
.globl combine1
	.type	combine1, @function
combine1:
.LFB2:
	pushq	%r13
.LCFI220:
	pushq	%r12
.LCFI221:
	movq	%rdi, %r12
	pushq	%rbp
.LCFI222:
	movq	%rsi, %rbp
	pushq	%rbx
.LCFI223:
	xorl	%ebx, %ebx
	subq	$24, %rsp
.LCFI224:
	movl	$0x3f800000, (%rsi)
	leaq	20(%rsp), %r13
	jmp	.L569
	.p2align 4,,10
	.p2align 3
.L570:
	movq	%rbx, %rsi
	movq	%r13, %rdx
	movq	%r12, %rdi
	call	get_vec_element
	movss	(%rbp), %xmm0
	addq	$1, %rbx
	mulss	20(%rsp), %xmm0
	movss	%xmm0, (%rbp)
.L569:
	movq	%r12, %rdi
	call	vec_length
	cmpq	%rax, %rbx
	jl	.L570
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	ret
.LFE2:
	.size	combine1, .-combine1
.globl combine1_descr
	.data
	.align 32
	.type	combine1_descr, @object
	.size	combine1_descr, 42
combine1_descr:
	.string	"combine1: Maximum use of data abstraction"
.globl combine2_descr
	.align 32
	.type	combine2_descr, @object
	.size	combine2_descr, 40
combine2_descr:
	.string	"combine2: Take vec_length() out of loop"
.globl combine3_descr
	.align 32
	.type	combine3_descr, @object
	.size	combine3_descr, 41
combine3_descr:
	.string	"combine3: Array reference to vector data"
.globl combine3w_descr
	.align 32
	.type	combine3w_descr, @object
	.size	combine3w_descr, 52
combine3w_descr:
	.string	"combine3w: Update *dest within loop only with write"
.globl combine4_descr
	.align 32
	.type	combine4_descr, @object
	.size	combine4_descr, 51
combine4_descr:
	.string	"combine4: Array reference, accumulate in temporary"
.globl combine4b_descr
	.align 32
	.type	combine4b_descr, @object
	.size	combine4b_descr, 39
combine4b_descr:
	.string	"combine4b: Include bonds check in loop"
.globl combine4p_descr
	.align 32
	.type	combine4p_descr, @object
	.size	combine4p_descr, 54
combine4p_descr:
	.string	"combine4p: Pointer reference, accumulate in temporary"
.globl combine5_descr
	.align 32
	.type	combine5_descr, @object
	.size	combine5_descr, 36
combine5_descr:
	.string	"combine5: Array code, unrolled by 2"
.globl unroll3a_descr
	.align 32
	.type	unroll3a_descr, @object
	.size	unroll3a_descr, 36
unroll3a_descr:
	.string	"unroll3a: Array code, unrolled by 3"
.globl combine5p_descr
	.align 32
	.type	combine5p_descr, @object
	.size	combine5p_descr, 49
combine5p_descr:
	.string	"combine5p: Pointer code, unrolled by 2, for loop"
.globl unroll2aw_descr
	.align 32
	.type	unroll2aw_descr, @object
	.size	unroll2aw_descr, 49
unroll2aw_descr:
	.string	"unroll2aw: Array code, unrolled by 2, while loop"
.globl unroll4a_descr
	.align 32
	.type	unroll4a_descr, @object
	.size	unroll4a_descr, 36
unroll4a_descr:
	.string	"unroll4a: Array code, unrolled by 4"
.globl unroll5a_descr
	.align 32
	.type	unroll5a_descr, @object
	.size	unroll5a_descr, 36
unroll5a_descr:
	.string	"unroll5a: Array code, unrolled by 5"
.globl unroll6a_descr
	.align 32
	.type	unroll6a_descr, @object
	.size	unroll6a_descr, 36
unroll6a_descr:
	.string	"unroll6a: Array code, unrolled by 6"
.globl unroll8a_descr
	.align 32
	.type	unroll8a_descr, @object
	.size	unroll8a_descr, 36
unroll8a_descr:
	.string	"unroll8a: Array code, unrolled by 8"
.globl unroll16a_descr
	.align 32
	.type	unroll16a_descr, @object
	.size	unroll16a_descr, 38
unroll16a_descr:
	.string	"unroll16a: Array code, unrolled by 16"
.globl unroll2_descr
	.align 32
	.type	unroll2_descr, @object
	.size	unroll2_descr, 37
unroll2_descr:
	.string	"unroll2: Pointer code, unrolled by 2"
.globl unroll3_descr
	.align 32
	.type	unroll3_descr, @object
	.size	unroll3_descr, 37
unroll3_descr:
	.string	"unroll3: Pointer code, unrolled by 3"
.globl unroll4_descr
	.align 32
	.type	unroll4_descr, @object
	.size	unroll4_descr, 37
unroll4_descr:
	.string	"unroll4: Pointer code, unrolled by 4"
.globl unroll8_descr
	.align 32
	.type	unroll8_descr, @object
	.size	unroll8_descr, 37
unroll8_descr:
	.string	"unroll8: Pointer code, unrolled by 8"
.globl unroll16_descr
	.align 32
	.type	unroll16_descr, @object
	.size	unroll16_descr, 39
unroll16_descr:
	.string	"unroll16: Pointer code, unrolled by 16"
.globl combine6_descr
	.align 32
	.type	combine6_descr, @object
	.size	combine6_descr, 52
combine6_descr:
	.string	"combine6: Array code, unrolled by 2, Superscalar x2"
.globl unroll4x2a_descr
	.align 32
	.type	unroll4x2a_descr, @object
	.size	unroll4x2a_descr, 54
unroll4x2a_descr:
	.string	"unroll4x2a: Array code, unrolled by 4, Superscalar x2"
.globl unroll8x2a_descr
	.align 32
	.type	unroll8x2a_descr, @object
	.size	unroll8x2a_descr, 54
unroll8x2a_descr:
	.string	"unroll8x2a: Array code, unrolled by 8, Superscalar x2"
.globl unroll3x3a_descr
	.align 32
	.type	unroll3x3a_descr, @object
	.size	unroll3x3a_descr, 54
unroll3x3a_descr:
	.string	"unroll3x3a: Array code, unrolled by 3, Superscalar x3"
.globl unroll4x4a_descr
	.align 32
	.type	unroll4x4a_descr, @object
	.size	unroll4x4a_descr, 54
unroll4x4a_descr:
	.string	"unroll4x4a: Array code, unrolled by 4, Superscalar x4"
.globl unroll8x4a_descr
	.align 32
	.type	unroll8x4a_descr, @object
	.size	unroll8x4a_descr, 54
unroll8x4a_descr:
	.string	"unroll8x4a: Array code, unrolled by 8, Superscalar x4"
.globl unroll12x6a_descr
	.align 32
	.type	unroll12x6a_descr, @object
	.size	unroll12x6a_descr, 55
unroll12x6a_descr:
	.string	"unroll2x6a: Array code, unrolled by 12, Superscalar x6"
.globl unroll12x12a_descr
	.align 32
	.type	unroll12x12a_descr, @object
	.size	unroll12x12a_descr, 58
unroll12x12a_descr:
	.string	"unroll12x12a: Array code, unrolled by 12, Superscalar x12"
.globl unroll5x5a_descr
	.align 32
	.type	unroll5x5a_descr, @object
	.size	unroll5x5a_descr, 54
unroll5x5a_descr:
	.string	"unroll5x5a: Array code, unrolled by 5, Superscalar x5"
.globl unroll6x6a_descr
	.align 32
	.type	unroll6x6a_descr, @object
	.size	unroll6x6a_descr, 54
unroll6x6a_descr:
	.string	"unroll6x6a: Array code, unrolled by 6, Superscalar x6"
.globl unroll8x8a_descr
	.align 32
	.type	unroll8x8a_descr, @object
	.size	unroll8x8a_descr, 54
unroll8x8a_descr:
	.string	"unroll8x8a: Array code, unrolled by 8, Superscalar x8"
.globl unroll10x10a_descr
	.align 32
	.type	unroll10x10a_descr, @object
	.size	unroll10x10a_descr, 58
unroll10x10a_descr:
	.string	"unroll10x10a: Array code, unrolled by 10, Superscalar x10"
.globl unrollx2as_descr
	.align 32
	.type	unrollx2as_descr, @object
	.size	unrollx2as_descr, 66
unrollx2as_descr:
	.string	"unrollx2as: Array code, Unroll x2, Superscalar x2, noninterleaved"
.globl unroll4x2as_descr
	.align 32
	.type	unroll4x2as_descr, @object
	.size	unroll4x2as_descr, 67
unroll4x2as_descr:
	.string	"unroll4x2as: Array code, Unroll x4, Superscalar x2, noninterleaved"
.globl unroll8x2_descr
	.align 32
	.type	unroll8x2_descr, @object
	.size	unroll8x2_descr, 55
unroll8x2_descr:
	.string	"unroll8x2: Pointer code, unrolled by 8, Superscalar x2"
.globl unroll9x3_descr
	.align 32
	.type	unroll9x3_descr, @object
	.size	unroll9x3_descr, 55
unroll9x3_descr:
	.string	"unroll9x3: Pointer code, unrolled by 9, Superscalar x3"
.globl unroll8x4_descr
	.align 32
	.type	unroll8x4_descr, @object
	.size	unroll8x4_descr, 55
unroll8x4_descr:
	.string	"unroll8x4: Pointer code, unrolled by 8, Superscalar x4"
.globl unroll8x8_descr
	.align 32
	.type	unroll8x8_descr, @object
	.size	unroll8x8_descr, 55
unroll8x8_descr:
	.string	"unroll8x8: Pointer code, unrolled by 8, Superscalar x8"
.globl combine7_descr
	.align 32
	.type	combine7_descr, @object
	.size	combine7_descr, 61
combine7_descr:
	.string	"combine7: Array code, unrolled by 2, different associativity"
.globl unroll3aa_descr
	.align 32
	.type	unroll3aa_descr, @object
	.size	unroll3aa_descr, 62
unroll3aa_descr:
	.string	"unroll3aa: Array code, unrolled by 3, Different Associativity"
.globl unroll4aa_descr
	.align 32
	.type	unroll4aa_descr, @object
	.size	unroll4aa_descr, 62
unroll4aa_descr:
	.string	"unroll4aa: Array code, unrolled by 4, Different Associativity"
.globl unroll5aa_descr
	.align 32
	.type	unroll5aa_descr, @object
	.size	unroll5aa_descr, 62
unroll5aa_descr:
	.string	"unroll5aa: Array code, unrolled by 5, Different Associativity"
.globl unroll6aa_descr
	.align 32
	.type	unroll6aa_descr, @object
	.size	unroll6aa_descr, 62
unroll6aa_descr:
	.string	"unroll6aa: Array code, unrolled by 6, Different Associativity"
.globl unroll8aa_descr
	.align 32
	.type	unroll8aa_descr, @object
	.size	unroll8aa_descr, 62
unroll8aa_descr:
	.string	"unroll8aa: Array code, unrolled by 8, Different Associativity"
.globl simd_v1_descr
	.align 32
	.type	simd_v1_descr, @object
	.size	simd_v1_descr, 43
simd_v1_descr:
	.string	"simd_v1: SSE code, 1*VSIZE-way parallelism"
.globl simd_v2_descr
	.align 32
	.type	simd_v2_descr, @object
	.size	simd_v2_descr, 43
simd_v2_descr:
	.string	"simd_v2: SSE code, 2*VSIZE-way parallelism"
.globl simd_v4_descr
	.align 32
	.type	simd_v4_descr, @object
	.size	simd_v4_descr, 43
simd_v4_descr:
	.string	"simd_v4: SSE code, 4*VSIZE-way parallelism"
.globl simd_v8_descr
	.align 32
	.type	simd_v8_descr, @object
	.size	simd_v8_descr, 43
simd_v8_descr:
	.string	"simd_v8: SSE code, 8*VSIZE-way parallelism"
.globl simd_v12_descr
	.align 32
	.type	simd_v12_descr, @object
	.size	simd_v12_descr, 45
simd_v12_descr:
	.string	"simd_v12: SSE code, 12*VSIZE-way parallelism"
.globl simd_v2a_descr
	.align 32
	.type	simd_v2a_descr, @object
	.size	simd_v2a_descr, 57
simd_v2a_descr:
	.string	"simd_v2a: SSE code, 2*VSIZE-way parallelism, reassociate"
.globl simd_v4a_descr
	.align 32
	.type	simd_v4a_descr, @object
	.size	simd_v4a_descr, 57
simd_v4a_descr:
	.string	"simd_v4a: SSE code, 4*VSIZE-way parallelism, reassociate"
.globl simd_v8a_descr
	.align 32
	.type	simd_v8a_descr, @object
	.size	simd_v8a_descr, 57
simd_v8a_descr:
	.string	"simd_v8a: SSE code, 8*VSIZE-way parallelism, reassociate"
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	3951369912
	.long	1070512209
	.align 8
.LC1:
	.long	1202590843
	.long	1069841121
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC2:
	.long	1065353216
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
	.long	.LFB55
	.long	.LFE55-.LFB55
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB55
	.byte	0xe
	.uleb128 0x10
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB54
	.long	.LFE54-.LFB54
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI1-.LFB54
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB53
	.long	.LFE53-.LFB53
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI5-.LFB53
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI7-.LCFI6
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI8-.LCFI7
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE5:
.LSFDE7:
	.long	.LEFDE7-.LASFDE7
.LASFDE7:
	.long	.LASFDE7-.Lframe1
	.long	.LFB52
	.long	.LFE52-.LFB52
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI9-.LFB52
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI10-.LCFI9
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI11-.LCFI10
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE7:
.LSFDE9:
	.long	.LEFDE9-.LASFDE9
.LASFDE9:
	.long	.LASFDE9-.Lframe1
	.long	.LFB51
	.long	.LFE51-.LFB51
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI13-.LFB51
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI15-.LCFI14
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE9:
.LSFDE11:
	.long	.LEFDE11-.LASFDE11
.LASFDE11:
	.long	.LASFDE11-.Lframe1
	.long	.LFB50
	.long	.LFE50-.LFB50
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI17-.LFB50
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI18-.LCFI17
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI19-.LCFI18
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE11:
.LSFDE13:
	.long	.LEFDE13-.LASFDE13
.LASFDE13:
	.long	.LASFDE13-.Lframe1
	.long	.LFB49
	.long	.LFE49-.LFB49
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI21-.LFB49
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE13:
.LSFDE15:
	.long	.LEFDE15-.LASFDE15
.LASFDE15:
	.long	.LASFDE15-.Lframe1
	.long	.LFB48
	.long	.LFE48-.LFB48
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI25-.LFB48
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI26-.LCFI25
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI27-.LCFI26
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI28-.LCFI27
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE15:
.LSFDE17:
	.long	.LEFDE17-.LASFDE17
.LASFDE17:
	.long	.LASFDE17-.Lframe1
	.long	.LFB47
	.long	.LFE47-.LFB47
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI29-.LFB47
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI30-.LCFI29
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI31-.LCFI30
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI32-.LCFI31
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE17:
.LSFDE19:
	.long	.LEFDE19-.LASFDE19
.LASFDE19:
	.long	.LASFDE19-.Lframe1
	.long	.LFB46
	.long	.LFE46-.LFB46
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI33-.LFB46
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI34-.LCFI33
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI35-.LCFI34
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI36-.LCFI35
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI37-.LCFI36
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE19:
.LSFDE21:
	.long	.LEFDE21-.LASFDE21
.LASFDE21:
	.long	.LASFDE21-.Lframe1
	.long	.LFB45
	.long	.LFE45-.LFB45
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI38-.LFB45
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI39-.LCFI38
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI40-.LCFI39
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI41-.LCFI40
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI42-.LCFI41
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE21:
.LSFDE23:
	.long	.LEFDE23-.LASFDE23
.LASFDE23:
	.long	.LASFDE23-.Lframe1
	.long	.LFB44
	.long	.LFE44-.LFB44
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI43-.LFB44
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI44-.LCFI43
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI45-.LCFI44
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI46-.LCFI45
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI47-.LCFI46
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE23:
.LSFDE25:
	.long	.LEFDE25-.LASFDE25
.LASFDE25:
	.long	.LASFDE25-.Lframe1
	.long	.LFB43
	.long	.LFE43-.LFB43
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI48-.LFB43
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI49-.LCFI48
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI50-.LCFI49
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI51-.LCFI50
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI52-.LCFI51
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE25:
.LSFDE27:
	.long	.LEFDE27-.LASFDE27
.LASFDE27:
	.long	.LASFDE27-.Lframe1
	.long	.LFB42
	.long	.LFE42-.LFB42
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI53-.LFB42
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI54-.LCFI53
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI55-.LCFI54
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI56-.LCFI55
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI57-.LCFI56
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE27:
.LSFDE29:
	.long	.LEFDE29-.LASFDE29
.LASFDE29:
	.long	.LASFDE29-.Lframe1
	.long	.LFB41
	.long	.LFE41-.LFB41
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI58-.LFB41
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI59-.LCFI58
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI60-.LCFI59
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI61-.LCFI60
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI62-.LCFI61
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE29:
.LSFDE31:
	.long	.LEFDE31-.LASFDE31
.LASFDE31:
	.long	.LASFDE31-.Lframe1
	.long	.LFB40
	.long	.LFE40-.LFB40
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI63-.LFB40
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI64-.LCFI63
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI65-.LCFI64
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE31:
.LSFDE33:
	.long	.LEFDE33-.LASFDE33
.LASFDE33:
	.long	.LASFDE33-.Lframe1
	.long	.LFB39
	.long	.LFE39-.LFB39
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI66-.LFB39
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI67-.LCFI66
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI68-.LCFI67
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE33:
.LSFDE35:
	.long	.LEFDE35-.LASFDE35
.LASFDE35:
	.long	.LASFDE35-.Lframe1
	.long	.LFB38
	.long	.LFE38-.LFB38
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI69-.LFB38
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI70-.LCFI69
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI71-.LCFI70
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE35:
.LSFDE37:
	.long	.LEFDE37-.LASFDE37
.LASFDE37:
	.long	.LASFDE37-.Lframe1
	.long	.LFB37
	.long	.LFE37-.LFB37
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI72-.LFB37
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI73-.LCFI72
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI74-.LCFI73
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE37:
.LSFDE39:
	.long	.LEFDE39-.LASFDE39
.LASFDE39:
	.long	.LASFDE39-.Lframe1
	.long	.LFB36
	.long	.LFE36-.LFB36
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI75-.LFB36
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI76-.LCFI75
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI77-.LCFI76
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI78-.LCFI77
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI79-.LCFI78
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE39:
.LSFDE41:
	.long	.LEFDE41-.LASFDE41
.LASFDE41:
	.long	.LASFDE41-.Lframe1
	.long	.LFB35
	.long	.LFE35-.LFB35
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI80-.LFB35
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI81-.LCFI80
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI82-.LCFI81
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI83-.LCFI82
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI84-.LCFI83
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE41:
.LSFDE43:
	.long	.LEFDE43-.LASFDE43
.LASFDE43:
	.long	.LASFDE43-.Lframe1
	.long	.LFB34
	.long	.LFE34-.LFB34
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI85-.LFB34
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI86-.LCFI85
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI87-.LCFI86
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE43:
.LSFDE45:
	.long	.LEFDE45-.LASFDE45
.LASFDE45:
	.long	.LASFDE45-.Lframe1
	.long	.LFB33
	.long	.LFE33-.LFB33
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI88-.LFB33
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI89-.LCFI88
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI90-.LCFI89
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI91-.LCFI90
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI92-.LCFI91
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE45:
.LSFDE47:
	.long	.LEFDE47-.LASFDE47
.LASFDE47:
	.long	.LASFDE47-.Lframe1
	.long	.LFB32
	.long	.LFE32-.LFB32
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI93-.LFB32
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI94-.LCFI93
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI95-.LCFI94
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI96-.LCFI95
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI97-.LCFI96
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE47:
.LSFDE49:
	.long	.LEFDE49-.LASFDE49
.LASFDE49:
	.long	.LASFDE49-.Lframe1
	.long	.LFB31
	.long	.LFE31-.LFB31
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI98-.LFB31
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI99-.LCFI98
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI100-.LCFI99
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI101-.LCFI100
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI102-.LCFI101
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE49:
.LSFDE51:
	.long	.LEFDE51-.LASFDE51
.LASFDE51:
	.long	.LASFDE51-.Lframe1
	.long	.LFB30
	.long	.LFE30-.LFB30
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI103-.LFB30
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI104-.LCFI103
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI105-.LCFI104
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE51:
.LSFDE53:
	.long	.LEFDE53-.LASFDE53
.LASFDE53:
	.long	.LASFDE53-.Lframe1
	.long	.LFB29
	.long	.LFE29-.LFB29
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI106-.LFB29
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI107-.LCFI106
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI108-.LCFI107
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI109-.LCFI108
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI110-.LCFI109
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE53:
.LSFDE55:
	.long	.LEFDE55-.LASFDE55
.LASFDE55:
	.long	.LASFDE55-.Lframe1
	.long	.LFB28
	.long	.LFE28-.LFB28
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI111-.LFB28
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI112-.LCFI111
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI113-.LCFI112
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI114-.LCFI113
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI115-.LCFI114
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE55:
.LSFDE57:
	.long	.LEFDE57-.LASFDE57
.LASFDE57:
	.long	.LASFDE57-.Lframe1
	.long	.LFB27
	.long	.LFE27-.LFB27
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI116-.LFB27
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI117-.LCFI116
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI118-.LCFI117
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI119-.LCFI118
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI120-.LCFI119
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE57:
.LSFDE59:
	.long	.LEFDE59-.LASFDE59
.LASFDE59:
	.long	.LASFDE59-.Lframe1
	.long	.LFB26
	.long	.LFE26-.LFB26
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI121-.LFB26
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI122-.LCFI121
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI123-.LCFI122
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI124-.LCFI123
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI125-.LCFI124
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE59:
.LSFDE61:
	.long	.LEFDE61-.LASFDE61
.LASFDE61:
	.long	.LASFDE61-.Lframe1
	.long	.LFB25
	.long	.LFE25-.LFB25
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI126-.LFB25
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI127-.LCFI126
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI128-.LCFI127
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI129-.LCFI128
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI130-.LCFI129
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE61:
.LSFDE63:
	.long	.LEFDE63-.LASFDE63
.LASFDE63:
	.long	.LASFDE63-.Lframe1
	.long	.LFB24
	.long	.LFE24-.LFB24
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI131-.LFB24
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI132-.LCFI131
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI133-.LCFI132
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI134-.LCFI133
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI135-.LCFI134
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE63:
.LSFDE65:
	.long	.LEFDE65-.LASFDE65
.LASFDE65:
	.long	.LASFDE65-.Lframe1
	.long	.LFB23
	.long	.LFE23-.LFB23
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI136-.LFB23
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI137-.LCFI136
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI138-.LCFI137
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI139-.LCFI138
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI140-.LCFI139
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE65:
.LSFDE67:
	.long	.LEFDE67-.LASFDE67
.LASFDE67:
	.long	.LASFDE67-.Lframe1
	.long	.LFB22
	.long	.LFE22-.LFB22
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI141-.LFB22
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI142-.LCFI141
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI143-.LCFI142
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE67:
.LSFDE69:
	.long	.LEFDE69-.LASFDE69
.LASFDE69:
	.long	.LASFDE69-.Lframe1
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI144-.LFB21
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI145-.LCFI144
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI146-.LCFI145
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE69:
.LSFDE71:
	.long	.LEFDE71-.LASFDE71
.LASFDE71:
	.long	.LASFDE71-.Lframe1
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI147-.LFB20
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI148-.LCFI147
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI149-.LCFI148
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE71:
.LSFDE73:
	.long	.LEFDE73-.LASFDE73
.LASFDE73:
	.long	.LASFDE73-.Lframe1
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI150-.LFB19
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI151-.LCFI150
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI152-.LCFI151
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE73:
.LSFDE75:
	.long	.LEFDE75-.LASFDE75
.LASFDE75:
	.long	.LASFDE75-.Lframe1
	.long	.LFB18
	.long	.LFE18-.LFB18
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI153-.LFB18
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI154-.LCFI153
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI155-.LCFI154
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE75:
.LSFDE77:
	.long	.LEFDE77-.LASFDE77
.LASFDE77:
	.long	.LASFDE77-.Lframe1
	.long	.LFB17
	.long	.LFE17-.LFB17
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI156-.LFB17
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI157-.LCFI156
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI158-.LCFI157
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI159-.LCFI158
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI160-.LCFI159
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE77:
.LSFDE79:
	.long	.LEFDE79-.LASFDE79
.LASFDE79:
	.long	.LASFDE79-.Lframe1
	.long	.LFB16
	.long	.LFE16-.LFB16
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI161-.LFB16
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI162-.LCFI161
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI163-.LCFI162
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI164-.LCFI163
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI165-.LCFI164
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE79:
.LSFDE81:
	.long	.LEFDE81-.LASFDE81
.LASFDE81:
	.long	.LASFDE81-.Lframe1
	.long	.LFB15
	.long	.LFE15-.LFB15
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI166-.LFB15
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI167-.LCFI166
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI168-.LCFI167
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI169-.LCFI168
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI170-.LCFI169
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE81:
.LSFDE83:
	.long	.LEFDE83-.LASFDE83
.LASFDE83:
	.long	.LASFDE83-.Lframe1
	.long	.LFB14
	.long	.LFE14-.LFB14
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI171-.LFB14
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI172-.LCFI171
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI173-.LCFI172
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI174-.LCFI173
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI175-.LCFI174
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE83:
.LSFDE85:
	.long	.LEFDE85-.LASFDE85
.LASFDE85:
	.long	.LASFDE85-.Lframe1
	.long	.LFB13
	.long	.LFE13-.LFB13
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI176-.LFB13
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI177-.LCFI176
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI178-.LCFI177
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI179-.LCFI178
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI180-.LCFI179
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE85:
.LSFDE87:
	.long	.LEFDE87-.LASFDE87
.LASFDE87:
	.long	.LASFDE87-.Lframe1
	.long	.LFB12
	.long	.LFE12-.LFB12
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI181-.LFB12
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI182-.LCFI181
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI183-.LCFI182
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI184-.LCFI183
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI185-.LCFI184
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE87:
.LSFDE89:
	.long	.LEFDE89-.LASFDE89
.LASFDE89:
	.long	.LASFDE89-.Lframe1
	.long	.LFB11
	.long	.LFE11-.LFB11
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI186-.LFB11
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI187-.LCFI186
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI188-.LCFI187
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE89:
.LSFDE91:
	.long	.LEFDE91-.LASFDE91
.LASFDE91:
	.long	.LASFDE91-.Lframe1
	.long	.LFB10
	.long	.LFE10-.LFB10
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI189-.LFB10
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI190-.LCFI189
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI191-.LCFI190
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI192-.LCFI191
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI193-.LCFI192
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE91:
.LSFDE93:
	.long	.LEFDE93-.LASFDE93
.LASFDE93:
	.long	.LASFDE93-.Lframe1
	.long	.LFB9
	.long	.LFE9-.LFB9
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI194-.LFB9
	.byte	0xe
	.uleb128 0x10
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI195-.LCFI194
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI196-.LCFI195
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI197-.LCFI196
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x8c
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI198-.LCFI197
	.byte	0xe
	.uleb128 0x30
	.align 8
.LEFDE93:
.LSFDE95:
	.long	.LEFDE95-.LASFDE95
.LASFDE95:
	.long	.LASFDE95-.Lframe1
	.long	.LFB8
	.long	.LFE8-.LFB8
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI199-.LFB8
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI200-.LCFI199
	.byte	0xe
	.uleb128 0x18
	.byte	0x86
	.uleb128 0x3
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI201-.LCFI200
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.align 8
.LEFDE95:
.LSFDE97:
	.long	.LEFDE97-.LASFDE97
.LASFDE97:
	.long	.LASFDE97-.Lframe1
	.long	.LFB7
	.long	.LFE7-.LFB7
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI202-.LFB7
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI203-.LCFI202
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI204-.LCFI203
	.byte	0xe
	.uleb128 0x20
	.align 8
.LEFDE97:
.LSFDE99:
	.long	.LEFDE99-.LASFDE99
.LASFDE99:
	.long	.LASFDE99-.Lframe1
	.long	.LFB6
	.long	.LFE6-.LFB6
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI205-.LFB6
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI206-.LCFI205
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI207-.LCFI206
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE99:
.LSFDE101:
	.long	.LEFDE101-.LASFDE101
.LASFDE101:
	.long	.LASFDE101-.Lframe1
	.long	.LFB5
	.long	.LFE5-.LFB5
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI208-.LFB5
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI209-.LCFI208
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI210-.LCFI209
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE101:
.LSFDE103:
	.long	.LEFDE103-.LASFDE103
.LASFDE103:
	.long	.LASFDE103-.Lframe1
	.long	.LFB4
	.long	.LFE4-.LFB4
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI211-.LFB4
	.byte	0xe
	.uleb128 0x10
	.byte	0x8c
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI212-.LCFI211
	.byte	0xe
	.uleb128 0x18
	.byte	0x4
	.long	.LCFI213-.LCFI212
	.byte	0xe
	.uleb128 0x20
	.byte	0x83
	.uleb128 0x4
	.byte	0x86
	.uleb128 0x3
	.align 8
.LEFDE103:
.LSFDE105:
	.long	.LEFDE105-.LASFDE105
.LASFDE105:
	.long	.LASFDE105-.Lframe1
	.long	.LFB3
	.long	.LFE3-.LFB3
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI214-.LFB3
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI215-.LCFI214
	.byte	0xe
	.uleb128 0x18
	.byte	0x8d
	.uleb128 0x3
	.byte	0x8e
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI216-.LCFI215
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.long	.LCFI217-.LCFI216
	.byte	0xe
	.uleb128 0x28
	.byte	0x86
	.uleb128 0x5
	.byte	0x8c
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI218-.LCFI217
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.long	.LCFI219-.LCFI218
	.byte	0xe
	.uleb128 0x40
	.byte	0x83
	.uleb128 0x6
	.align 8
.LEFDE105:
.LSFDE107:
	.long	.LEFDE107-.LASFDE107
.LASFDE107:
	.long	.LASFDE107-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI220-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x4
	.long	.LCFI221-.LCFI220
	.byte	0xe
	.uleb128 0x18
	.byte	0x8c
	.uleb128 0x3
	.byte	0x8d
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI222-.LCFI221
	.byte	0xe
	.uleb128 0x20
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI223-.LCFI222
	.byte	0xe
	.uleb128 0x28
	.byte	0x83
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI224-.LCFI223
	.byte	0xe
	.uleb128 0x40
	.align 8
.LEFDE107:
	.ident	"GCC: (Ubuntu 4.3.2-1ubuntu11) 4.3.2"
	.section	.note.GNU-stack,"",@progbits
