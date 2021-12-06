#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

namespace conway
{

  // On "importe" des types pour ne pas a avoir a reecrire std::.
  // Notez que les using sont propres au namespace conway
  // et pas au namespace general.

  using std::size_t;
  using std::uint8_t;

  //
  // Definitions de types
  //

  /// Etat d'une cellule
  enum cell_state_t : bool
  {
    dead = false,
    alive = true,
  };

  /// Representation d'une grille de taille M * N (nb de lignes * nb de colonnes)
  template <size_t M, size_t N>
  using grid_t = std::array<std::array<cell_state_t, N>, M>;

  //
  // Definitions de fonctions
  //

  /// Une fonction qui init une grille de 80 * 80 avec un glider
  constexpr grid_t<80, 80> make_grid()
  {
    conway::grid_t<80, 80> grid;

    // Zero init

    for (auto &line : grid)
    {
      for (auto &cell : line)
      {
        cell = conway::dead;
      }
    }

    // Glider init

    grid[10][11] = conway::alive;

    grid[11][12] = conway::alive;

    grid[12][10] = conway::alive;
    grid[12][11] = conway::alive;
    grid[12][12] = conway::alive;

    return grid;
  }

  /// Une fonction print qui renvoie une string qui pour chaque cellule contient
  /// '#' si elle est vivante, sinon un espace.
  template <size_t M, size_t N>
  std::string print(grid_t<M, N> const &grid)
  {
    std::string str;

    for (size_t i = 0; i < M; ++i)
    {
      for (size_t j = 0; j < N; ++j)
      {
        str += grid[i][j] ? '#' : ' ';
      }
      str += '\n';
    }

    return str;
  }

  /// Renvoie le nouvel etat d'une cellule en fonction de son nombre de voisins.
  cell_state_t get_state(cell_state_t previous_state,
                         uint8_t neighbor_count)
  {
    if (previous_state == conway::alive)
    {
      return neighbor_count == 2 || neighbor_count == 3 ? conway::alive
                                                        : conway::dead;
    }
    else
    {
      return neighbor_count == 3 ? conway::alive : conway::dead;
    }
  }

  /// Compte le nombre de voisins d'une cellule en supposant
  /// qu'elle ne puisse pas toucher un bord de la grille.
  template <size_t M, size_t N>
  uint8_t inner_neighbor_count(grid_t<M, N> const &grid,
                               size_t i, size_t j)
  {
    uint8_t count = 0;
    for (size_t k = i - 1; k <= i + 1; ++k)
    {
      for (size_t l = j - 1; l <= j + 1; ++l)
      {
        if (k != i || l != j)
        {
          count += grid[k][l];
        }
      }
    }
    return count;
  }

  /// Compte le nombre de voisins d'une cellule en supposant
  /// qu'elle puisse toucher un bord de la grille.
  template <size_t M, size_t N>
  uint8_t outer_neighbor_count(const grid_t<M, N> &grid,
                               size_t i, size_t j)
  {
    uint8_t res = 0;
    for (size_t k = 0; k < 3; ++k)
    {
      for (size_t l = 0; l < 3; ++l)
      {
        if (k == 1 && l == 1)
        {
          continue;
        }
        if (i + k < M && j + l < N)
        {
          res += grid[i + k][j + l];
        }
      }
    }

    return res;
  }

  /// Effectue le calcul du nouvel etat d'une cellule
  /// qui ne touche aucun bord de la grille.
  template <size_t M, size_t N>
  constexpr cell_state_t inner_conway(const grid_t<M, N> &grid,
                                      size_t i, size_t j)
  {
    return get_state(grid[i][j], inner_neighbor_count(grid, i, j));
  }

  /// Effectue le calcul du nouvel etat d'une cellule
  /// qui touche un bord de la grille.
  template <size_t M, size_t N>
  constexpr cell_state_t outer_conway(const grid_t<M, N> &grid,
                                      size_t i, size_t j)
  {
    return get_state(grid[i][j], outer_neighbor_count(grid, i, j));
  }

  /// Prend une grille en entree et effectue une iteration
  /// du jeu de la vie de Conway.
  template <size_t M, size_t N>
  constexpr grid_t<M, N> conway(grid_t<M, N> const &grid)
  {
    grid_t<M, N> new_grid;
    //Iterate over the grid
    for (size_t i = 0; i < M; ++i)
    {
      for (size_t j = 0; j < N; ++j)
      {
        if(i==0 || i==M-1 || j==0 || j==N-1)
        {
          new_grid[i][j] = outer_conway(grid, i, j);
        }
        else
        {
          new_grid[i][j] = inner_conway(grid, i, j);
        }
      }
    }

    return new_grid;
  }

  // On se dit que ce serait chouette que tout ce code puisse tourner a la
  // compilation pour le fun... Si seulement il y avait un moyen...

  // Faites en sorte que le code compile toujours
  // lorsqu'on de-commente le bloc suivant:

  // template <auto grid_a, auto grid_b>
  // constexpr bool b_follows_a()
  // {
  //   return grid_b == conway(grid_a);
  // }

  // conway::grid_t<80, 80> constexpr grid_a = conway::make_grid();
  // conway::grid_t<80, 80> constexpr grid_b = conway::conway(grid_a);

  // static_assert(conway::b_follows_a<grid_a, grid_b>(),
  //               "Impossible pour moi de laisser compiler ce code.");

} // namespace conway
