#ifndef _BTF_ENCODER_H_
#define _BTF_ENCODER_H_ 1
/*
  SPDX-License-Identifier: GPL-2.0-only

  Copyright (C) 2019 Facebook

  Derived from ctf_encoder.h, which is:
  Copyright (C) Arnaldo Carvalho de Melo <acme@redhat.com>
 */

#include <stdbool.h>
#include <stdint.h>
#include <gelf.h>

struct btf;
struct btf_elf;
struct cu;
struct elf_symtab;
struct elf_function;

#define MAX_PERCPU_VAR_CNT 4096

struct var_info {
	uint64_t    addr;
	const char *name;
	uint32_t    sz;
};

struct btf_encoder {
	struct btf_elf *btfe;
	const char	  *filename;
	struct elf_symtab *symtab;
	GElf_Ehdr	  ehdr;
	bool		  has_index_type,
			  need_index_type,
			  verbose;
	uint32_t	  array_index_id;
	struct {
		struct var_info vars[MAX_PERCPU_VAR_CNT];
		int		var_cnt;
		uint32_t	shndx;
		uint64_t	base_addr;
		uint64_t	sec_sz;
	} percpu;
	struct {
		struct elf_function *entries;
		int		    allocated;
		int		    cnt;
	} functions;
};

extern struct btf_encoder *encoder;

struct btf_encoder *btf_encoder__new(struct cu *cu, struct btf *base_btf, bool skip_encoding_vars, bool verbose);
void btf_encoder__delete(struct btf_encoder *encoder);

int btf_encoder__encode(struct btf_encoder *encoder, const char *detached_filename);

int cu__encode_btf(struct cu *cu, int verbose, bool force, bool skip_encoding_vars);

#endif /* _BTF_ENCODER_H_ */
