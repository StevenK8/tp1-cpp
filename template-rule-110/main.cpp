#include <rule-110.hpp>

#include <cassert>
#include <iostream>
#include <string>

int main() {
  // Asserts pour la logique

  rule110::rule_t rule110_val({
      false, // 7
      true,  // 6
      true,  // 5
      true,  // 4
      false, // 3
      true,  // 2
      true,  // 1
      false, // 0
  });

  assert(rule110::uint8t_to_rule(110) == rule110_val);

  // Attention: l'ordre de lecture sous forme de tableau est inverse par rapport
  // a l'ordre de lecture sous forme d'un nombre binaire. Vous devez donc
  // inverser l'ordre de lecture entre les regles dans la definition wikipedia
  // et la representation sous forme d'un std::array.
  //
  // Ex: 110 <=> { false, true, true }

  assert(rule110::apply_rule(rule110_val, {true, true, false}));
  assert(!rule110::apply_rule(rule110_val, {true, true, true}));
  assert(rule110::apply_rule(rule110_val, {true, false, true}));
  assert(!rule110::apply_rule(rule110_val, {false, false, true}));

  // // Current pattern            111  110  101  100  011  010  001  000
  // // New state for center cell  0    1    1    0    1    1    1    0

  // Representation de 110
  rule110::pattern_t<14> const pattern_0({
      true,
      true,
      true,
      true,
      true,
      false,
      true,
      true,
      false,
      false,
      true,
      false,
      false,
      false,
  });

  auto pattern_1 = rule110::iterate(rule110_val, pattern_0);


  // std::cout << "Pattern 1: ";
  // for (auto const& cell : pattern_1) {
  //   std::cout << (cell ? "1" : "0");
  // }
  // std::cout << std::endl;

  // Representation de 00110111110001, cense etre l'etat suivant pattern_0
  rule110::pattern_t<14> const pattern_1_e({
      true,
      false,
      false,
      false,
      true,
      true,
      true,
      true,
      true,
      false,
      true,
      true,
      false,
      false,
  });

  // std::cout << "Pattern 2: ";
  // for (auto const& cell : pattern_1_e) {
  //   std::cout << (cell ? "1" : "0");
  // }

  // std::cout << "\n 1==2 ? " << (pattern_1 == pattern_1_e) << std::endl;

  assert(pattern_1 == pattern_1_e);

  // // Asserts pour la fonction print

  std::cout << print(rule110::template_pattern_t<true, true, false>{}) << '\n';
  assert(print(rule110::template_pattern_t<true, true, false>{}) == "## ");

  std::cout << print(rule110::template_pattern_t<true, true, true>{}) << '\n';
  assert(print(rule110::template_pattern_t<true, true, true>{}) == "###");

  std::cout << print(rule110::template_pattern_t<true, false, true>{}) << '\n';
  assert(print(rule110::template_pattern_t<true, false, true>{}) == "# #");

  static_assert((rule110::template_pattern_t<true, true, false>{} ==
                rule110::template_pattern_t<true, true, false>{}));

  static_assert(!(rule110::template_pattern_t<false, true, false>{} ==
                  rule110::template_pattern_t<true, true, false>{}));

  static_assert(!(rule110::template_pattern_t<false, true, false>{} ==
                  rule110::template_pattern_t<true, true, false, false>{}));

  static_assert(rule110::make_template_pattern<rule110::pattern_t<3>(
                    {true, true, false})>() ==
                rule110::template_pattern_t<true, true, false>{});

  // La routourne
  auto state = pattern_0;
  for (std::size_t i = 0; i < 64; i++) {
    std::cout << rule110::print(state) << '\n';
    state = rule110::iterate(rule110_val, state);
  }
}
