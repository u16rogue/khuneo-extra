#include <kh-extra/lexer.h>
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

    switch (token->type) {
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
    switch (token->type) {
      case KH_TOK_INVALID: {
        if (out_token_type_str_sz > 7) {
          kh_utf8_strcpy(out_token_value_str, "INVALID");
        }
        break;
      }
      case KH_TOK_STRING:
      case KH_TOK_IDENTIFIER: {
        // must be larger than the buffer so we can place a null
        if (out_token_type_str_sz > token->value.string.size) {
          int i = 0;
          for (; i < token->value.string.size; ++i) {
            out_token_value_str[i] = ctx->src[token->value.string.index + i];
          }
          if (i < out_token_value_str_sz) // Make sure that we dont write a null on oob
            out_token_value_str[i] = '\0';
        }
        break;
      }
      case KH_TOK_CHARSYM: {
        if (out_token_value_str_sz > 1) {
          out_token_value_str[0] = token->value.charsym;
          out_token_value_str[1] = '\0';
        }
        break;
      }
      case KH_TOK_U64: {
        snprintf(out_token_value_str, out_token_value_str_sz, "%llu", token->value.u64);
        break;
      }
      case KH_TOK_F64: {
        snprintf(out_token_value_str, out_token_value_str_sz, "%f", token->value.f64);
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
    snprintf(out_token_line_str, out_token_line_str_sz, "%u", token->line);
#else
    if (out_token_line_str > 1) {
      out_token_line_str[0] = '0';
      out_token_line_str[1] = '\0';
    }
#endif
  }

  if (out_token_column_str) {
#if defined(KH_TRACK_LINE_COLUMN) 
    snprintf(out_token_column_str, out_token_column_str_sz, "%u", token->column);
#else
    if (out_token_column_str_sz > 1) {
      out_token_column_str[0] = '0';
      out_token_column_str[1] = '\0';
    }
#endif
  }

  return 1;
}
