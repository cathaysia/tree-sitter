#include "compiler/prepare_grammar/prepare_grammar.h"
#include "compiler/prepared_grammar.h"
#include "compiler/prepare_grammar/extract_tokens.h"
#include "compiler/prepare_grammar/expand_repeats.h"
#include "compiler/prepare_grammar/expand_tokens.h"
#include "compiler/prepare_grammar/intern_symbols.h"
#include "compiler/prepare_grammar/interned_grammar.h"
#include "compiler/prepared_grammar.h"

namespace tree_sitter {
    using std::tuple;
    using std::make_tuple;

    namespace prepare_grammar {
        tuple<SyntaxGrammar, LexicalGrammar, const GrammarError *>
        prepare_grammar(const Grammar &input_grammar) {
            auto result = intern_symbols(input_grammar);
            const InternedGrammar &grammar = result.first;
            const GrammarError *error = result.second;

            if (error)
                return make_tuple(SyntaxGrammar(), LexicalGrammar(), error);

            auto grammars = extract_tokens(grammar);
            const SyntaxGrammar &rule_grammar = expand_repeats(grammars.first);
            auto expand_tokens_result = expand_tokens(grammars.second);
            const LexicalGrammar &lex_grammar = expand_tokens_result.first;
            error = expand_tokens_result.second;

            if (error)
                return make_tuple(SyntaxGrammar(), LexicalGrammar(), error);

            return make_tuple(rule_grammar, lex_grammar, nullptr);
        }
    }
}
