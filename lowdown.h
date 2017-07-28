/*	$Id$ */
/*
 * Copyright (c) 2017, Kristaps Dzonsons
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef LOWDOWN_H
#define LOWDOWN_H

/*
 * All of this is documented in lowdown.3.
 * If it's not documented, don't use it.
 * Or report it as a bug.
 */

/* We need this for compilation on musl systems. */

#ifndef __BEGIN_DECLS
# ifdef __cplusplus
#  define       __BEGIN_DECLS           extern "C" {
# else
#  define       __BEGIN_DECLS
# endif
#endif
#ifndef __END_DECLS
# ifdef __cplusplus
#  define       __END_DECLS             }
# else
#  define       __END_DECLS
# endif
#endif

enum	lowdown_type {
	LOWDOWN_HTML,
	LOWDOWN_MAN,
	LOWDOWN_NROFF
};

enum	lowdown_err {
	LOWDOWN_ERR_SPACE_BEFORE_LINK = 0,
	LOWDOWN_ERR_METADATA_BAD_CHAR,
	LOWDOWN_ERR__MAX
};

typedef	void (*lowdown_msg)(enum lowdown_err, void *, const char *);

enum	lowdown_rndrt {
	LOWDOWN_ROOT,
	LOWDOWN_BLOCKCODE,
	LOWDOWN_BLOCKQUOTE,
	LOWDOWN_HEADER,
	LOWDOWN_HRULE,
	LOWDOWN_LIST,
	LOWDOWN_LISTITEM,
	LOWDOWN_PARAGRAPH,
	LOWDOWN_TABLE,
	LOWDOWN_TABLE_HEADER,
	LOWDOWN_TABLE_BODY,
	LOWDOWN_TABLE_ROW,
	LOWDOWN_TABLE_CELL,
	LOWDOWN_FOOTNOTES,
	LOWDOWN_FOOTNOTE_DEF,
	LOWDOWN_BLOCKHTML,
	LOWDOWN_AUTOLINK,
	LOWDOWN_CODESPAN,
	LOWDOWN_DOUBLE_EMPHASIS,
	LOWDOWN_EMPHASIS,
	LOWDOWN_HIGHLIGHT,
	LOWDOWN_IMAGE,
	LOWDOWN_LINEBREAK,
	LOWDOWN_LINK,
	LOWDOWN_TRIPLE_EMPHASIS,
	LOWDOWN_STRIKETHROUGH,
	LOWDOWN_SUPERSCRIPT,
	LOWDOWN_FOOTNOTE_REF,
	LOWDOWN_MATH,
	LOWDOWN_RAW_HTML,
	LOWDOWN_ENTITY,
	LOWDOWN_NORMAL_TEXT,
	LOWDOWN_BACKSPACE,
	LOWDOWN_DOC_HEADER,
	LOWDOWN_DOC_FOOTER,
	LOWDOWN__MAX
};

TAILQ_HEAD(lowdown_nodeq, lowdown_node);

/*
 * Node parsed from input document.
 * Each node is part of the parse tree.
 */
struct	lowdown_node {
	enum lowdown_rndrt	 type;
	union {
		struct rndr_normal_text {
			char *text;
			size_t textsz;
		} rndr_normal_text; 
		struct rndr_entity {
			char *text;
			size_t textsz;
		} rndr_entity; 
		struct rndr_autolink {
			char *link;
			size_t linksz;
		} rndr_autolink; 
		struct rndr_raw_html {
			char *text;
			size_t textsz;
		} rndr_raw_html; 
		struct rndr_link {
			char *text;
			size_t textsz;
			char *link;
			size_t linksz;
		} rndr_link; 
		struct rndr_blockcode {
			char *text;
			size_t textsz;
			char *lang;
			size_t langsz;
		} rndr_blockcode; 
	};
	struct lowdown_node *parent;
	struct lowdown_nodeq children;
	TAILQ_ENTRY(lowdown_node) entries;
};

struct	lowdown_meta {
	char		*key;
	char		*value;
};

struct	lowdown_opts {
	lowdown_msg	  msg;
	enum lowdown_type type;
	void		 *arg;
	unsigned int	  feat;
#define LOWDOWN_TABLES	  0x01
#define LOWDOWN_FENCED	  0x02
#define LOWDOWN_FOOTNOTES 0x04
#define LOWDOWN_AUTOLINK  0x08
#define LOWDOWN_STRIKE	  0x10
#define LOWDOWN_HILITE	  0x40
#define LOWDOWN_SUPER	  0x100
#define LOWDOWN_MATH	  0x200
#define LOWDOWN_NOINTEM	  0x400
#define LOWDOWN_SPHD	  0x800
#define LOWDOWN_MATHEXP	  0x1000
#define LOWDOWN_NOCODEIND 0x2000
#define	LOWDOWN_METADATA  0x4000
	unsigned int	  oflags;
#define LOWDOWN_HTML_SKIP_HTML	0x01
#define LOWDOWN_HTML_ESCAPE	0x02
#define LOWDOWN_HTML_HARD_WRAP	0x04
#define LOWDOWN_NROFF_SKIP_HTML	0x08
#define LOWDOWN_NROFF_HARD_WRAP	0x10
#define LOWDOWN_NROFF_GROFF	0x20
#define LOWDOWN_NROFF_NUMBERED	0x80
#define	LOWDOWN_SMARTY	  	0x40
};

__BEGIN_DECLS

void		 lowdown_buf(const struct lowdown_opts *, 
			const unsigned char *, size_t,
			unsigned char **, size_t *,
			struct lowdown_meta **, size_t *);
const char	*lowdown_errstr(enum lowdown_err);
int		 lowdown_file(const struct lowdown_opts *, 
			FILE *, unsigned char **, size_t *,
			struct lowdown_meta **, size_t *);
void		 lowdown_standalone_open
			(const struct lowdown_opts *,
			 const struct lowdown_meta *, size_t,
			 unsigned char **, size_t *);
void		 lowdown_standalone_close
			(const struct lowdown_opts *,
			 unsigned char **, size_t *);

__END_DECLS

#endif /* !EXTERN_H */
