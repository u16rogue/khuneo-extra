#include <kh-extra/lexer.h>

#if __has_include(<kh-astgen/lexer.h>)

#include <kh-core/utf8.h>
#include <stdio.h>

kh_bool kh_extra_stringify_token_entry(
  const kh_lexer_context * ctx,
  const kh_lexer_token_entry * token,
  kh_utf8 * out_token_type_str,   kh_sz out_token_type_str_sz,
  kh_utf8 * out_token_value_str,  kh_sz out_token_value_str_sz,
  kh_utf8 * out_token_line_str,   kh_sz out_token_line_str_sz,
  kh_utf8 * out_token_column_str, kh_sz out_token_column_str_sz
) {

  if (out_token_type_str) {
    kh_utf8 * sel = "<undefined>"; 
    kh_i32    sz = 12;
    
    #define ttok_str(x)   \
      case x:             \
        sel = #x;         \
        sz  = sizeof(#x); \
        break

    switch (kh_lexer_token_entry_type_get(token)) {
      ttok_str(KH_TOK_INVALID);
      ttok_str(KH_TOK_IDENTIFIER);
      ttok_str(KH_TOK_STRING);
      ttok_str(KH_TOK_CHARSYM);
      ttok_str(KH_TOK_U64);
      ttok_str(KH_TOK_F64);
      //casestr(KH_TOK_KEYWORD);
    }

    #undef ttok_str

    if (out_token_type_str_sz >= sz) {
      kh_utf8_strcpy(out_token_type_str, sel);
    }
  }

  if (out_token_value_str) {
    switch (kh_lexer_token_entry_type_get(token)) {
      case KH_TOK_INVALID: {
        if (out_token_type_str_sz > 7) {
          kh_utf8_strcpy(out_token_value_str, "INVALID");
        }
        break;
      }
      case KH_TOK_STRING:
      case KH_TOK_IDENTIFIER: {
        // must be larger than the buffer so we can place a null
        const kh_sz strsz = kh_lexer_token_entry_value_str_sz_get(token);
        const kh_u32 strindex = kh_lexer_token_entry_value_str_index_get(token);
        if (out_token_type_str_sz > strsz) {
          int i = 0;
          for (; i < strsz; ++i) {
            out_token_value_str[i] = ctx->src[strindex + i];
          }
          if (i < out_token_value_str_sz) // Make sure that we dont write a null on oob
            out_token_value_str[i] = '\0';
        }
        break;
      }
      case KH_TOK_CHARSYM: {
        if (out_token_value_str_sz > 1) {
          out_token_value_str[0] = kh_lexer_token_entry_value_charsym_get(token);
          out_token_value_str[1] = '\0';
        }
        break;
      }
      case KH_TOK_U64: {
        snprintf(out_token_value_str, out_token_value_str_sz, "%llu", kh_lexer_token_entry_value_u64_get(token));
        break;
      }
      case KH_TOK_F64: {
        snprintf(out_token_value_str, out_token_value_str_sz, "%f", kh_lexer_token_entry_value_f64_get(token));
        break;
      }
      default: {
        break;
      }
      //casestr(KH_TOK_KEYWORD);
    }
  }

  if (out_token_line_str) {
#if defined(KH_TRACK_LINE_COLUMN) 
    snprintf(out_token_line_str, out_token_line_str_sz, "%u", kh_lexer_token_entry_line_get(token));
#else
    if (out_token_line_str > 1) {
      out_token_line_str[0] = '0';
      out_token_line_str[1] = '\0';
    }
#endif
  }

  if (out_token_column_str) {
#if defined(KH_TRACK_LINE_COLUMN) 
    snprintf(out_token_column_str, out_token_column_str_sz, "%u", kh_lexer_token_entry_column_get(token));
#else
    if (out_token_column_str_sz > 1) {
      out_token_column_str[0] = '0';
      out_token_column_str[1] = '\0';
    }
#endif
  }

  return 1;
}

#endif
