#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>

// On travaille sur l'implementation d'automates cellulaires 1D qui s'executent
// a la compilation (ou pas, au choix)
// Voir: https://en.wikipedia.org/wiki/Rule_110

namespace rule110
{

  // Represente une taille
  using std::size_t;

  // Represente une regle sous forme d'entier 8 bits non-signe
  // (eg. rule 110)
  using std::uint8_t;

  /// Represente une regle (eg. la regle 110) sous forme de serie de booleens.
  using rule_t = std::array<bool, 8>;

  /// Represente une serie de cellules
  template <size_t N>
  using pattern_t = std::array<bool, N>;

  /// Represente un sous-pattern de 3 cellules
  using subpattern_t = std::array<bool, 3>;

  // Utilisez std::bool_constant pour convertir un uint8_t en un rule_t. Vous
  // pouvez utiliser les operations bitwise >> et & pour obtenir le bit du uint8_t
  // que vous souhaitez observer.
  //
  // std::bool_constant:
  //  https://en.cppreference.com/w/cpp/types/integral_constant
  //
  // Bitwise operators:
  //  https://en.cppreference.com/w/cpp/language/operator_arithmetic

  //
  // Partie 1: La logique des automates
  //

  /// Extrait un uint8_t en une regle
  constexpr rule_t uint8t_to_rule(uint8_t i)
  {
    return {(i & 0x01) != 0, (i & 0x02) != 0, (i & 0x04) != 0,
            (i & 0x08) != 0, (i & 0x10) != 0, (i & 0x20) != 0,
            (i & 0x40) != 0, (i & 0x80) != 0};
  }

  /// Applique la regle rule_t pour un subpattern donne
  constexpr bool apply_rule(rule_t rule, subpattern_t const &vals)
  {
    return rule[(vals[2] << 2) | (vals[1] << 1) | vals[0]];
  }

  /// Effectue une iteration du jeu de la vie. Pour la gestion des bords,
  /// considerez les valeurs en-dehors du pattern en entree comme false.
  template <size_t N>
  constexpr pattern_t<N> iterate(rule_t rule, pattern_t<N> const &pattern)
  {
    static_assert(N >= 3, "Un pattern doit contenir au moins 3 etats.");
    pattern_t<N> next_pattern = pattern;
    for (size_t i = 0; i < N; i++)
    {
      if (i == 0)
      {
        next_pattern[i] = apply_rule(rule, {0, pattern[i],
                                            pattern[i + 1]});
      }
      else if (i == N - 1)
      {
        next_pattern[i] = apply_rule(rule, {pattern[i - 1], pattern[i],
                                            0});
      }
      else
      {
        next_pattern[i] = apply_rule(rule, {pattern[i - 1], pattern[i],
                                            pattern[i + 1]});
      }
    }
    // next_pattern[N - 2] = false;
    // next_pattern[N - 1] = false;
    // next_pattern[0] = true;
    // next_pattern[N] = false;
    return next_pattern;
  }

  //
  // Partie 2: constexpr programming <=> template metaprogramming
  //

  /// Representation template d'un pattern
  template <bool... Cells>
  struct template_pattern_t
  {
    static constexpr size_t size = sizeof...(Cells);
  };

  // Convertit un pattern_t en template_pattern_t
  template <auto pattern>
  constexpr auto make_template_pattern()
  {
    using Seq = std::make_index_sequence<pattern.size()>;
    return [&]<auto... val>(std::index_sequence<val...> const &)
    {
      return template_pattern_t<(pattern[val])...>{};
    }
    (Seq{});
  }

  /// Convertit un template_pattern_t en pattern_t
  template <bool... Cells>
  constexpr pattern_t<sizeof...(Cells)>
      make_pattern(template_pattern_t<Cells...>)
  {
    return pattern_t<sizeof...(Cells)>{Cells...};
  }

  /// Print renvoie une std::string qui contient successivement pour chaque
  /// cellule '#' si elle est vivante, sinon un espace.
  template <bool... Cells>
  std::string print(template_pattern_t<Cells...>)
  {
    std::string res;
    res.reserve(sizeof...(Cells));
    (void)std::initializer_list<int>{
        (res += Cells ? '#' : ' ', 0)...};
    return res;
  }

  /// Print renvoie une std::string qui contient successivement pour chaque
  /// cellule '#' si elle est vivante, sinon un espace.
  template <size_t N>
  std::string print(pattern_t<N> const &pattern)
  {
    std::string res;
    for (size_t i = 0; i < N; i++)
    {
      if (pattern[i])
        res += '#';
      else
        res += ' ';
    }
    return res;
  }

  /// L'operator== entre deux template_pattern_t verifie que les deux contiennent
  /// les memes valeurs. Si les deux ne sont pas de la meme taille, la fonction ne
  /// doit pas crasher mais renvoyer false.
  ///
  /// NB: un if constexpr dont la condition est fausse peut contenir du code qui
  /// ne devrait pas forcement compiler...
  template <bool... CellsA, bool... CellsB>
  constexpr bool operator==(template_pattern_t<CellsA...>,
                            template_pattern_t<CellsB...>)
  {
    if constexpr (sizeof...(CellsA) != sizeof...(CellsB))
    {
      return false;
    }
    else
    {
      int res = 0;
      ((res += (CellsA == CellsB ? 1 : 0)), ...);
      return res == sizeof...(CellsA);
    }
  }

} // namespace rule110
