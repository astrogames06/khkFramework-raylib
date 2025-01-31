/*
  license under zlib license
  Copyright (C) 2020-2021 Kevin Haryo Kuncoro

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original software.

  3. This notice may not be removed or altered from any source distribution.

  Kevin Haryo Kuncoro
  kevinhyokun91@gmail.com
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include<Node.h>
#include<File.h>
#include<TextureAtlas.h>
#include<CCamera.h>

struct Grid{
  int width = 0;
  int height = 0;
  int column = 0;
  int row = 0;
};

namespace Component{

  /*
    Available component:
    - SpriteRenderer
    - Animator
    - AtlasAnimator
    - Tilemap
    - TMXMap
  */

  /*
    NOTE[kevin]:
    PlayAnim function on animator and atlas animator component only
    run if texture != nullptr.
  */

  struct BaseComponent{
    bool is_enable = true;

    void Attach();
    void SetNode(Node *_node);
    Node* GetNode();
  protected:
    Node *node = nullptr; // component owner

    virtual void _OnAttach();
  };

  struct DrawableBaseComponent : BaseComponent{
    Texture2D *texture = nullptr;
    int sorting_order = 0;
    bool is_visible = true;
    void Draw(int layer_index = -1); // draw when component enable and visible

  protected:
    Vector2 pivot{0.0f, 0.0f};
    Rectangle src_rect{0.0f, 0.0f, 0.0f, 0.0f};
    Rectangle dst_rect{0.0f, 0.0f, 0.0f, 0.0f};

    virtual void _OnDraw(int layer_index);
  };

  struct GridBaseComponent : DrawableBaseComponent{
    Grid* GetGrid();

  protected:
    Grid *grid = nullptr;

    Rectangle _GetSrcRect(int tile);
  };

  struct BaseTilemap : GridBaseComponent{
    int GetMaxWidth();
    int GetMaxHeight();
    v2 GetGridPosition(int column, int row);

  protected:
    int max_width = 0;
    int max_height = 0;

    int _GetIndex(int column, int row);
  };


  /*
    SpriteRenderer Component
  */
  typedef struct SpriteRenderer : DrawableBaseComponent{
    SpriteRenderer(Texture2D *_texture = nullptr);

  protected:
    void _OnDraw(int layer_index) override;
  } *sprite_renderer;


  /*
    Animator Component
  */
  typedef struct Animator : GridBaseComponent{
    Animator(int frame_width, int frame_height, Texture2D *_texture = nullptr);
    void PlayAnim(vector<int> anim_frame, int fps);

  protected:
    int frame_index = 0;
    int frame_counter = 0;

    void _OnDraw(int layer_index);
  } *animator;


  /*
    AtlasAnimator Component
  */
  typedef struct AtlasAnimator : DrawableBaseComponent{
    AtlasAnimator(TextureAtlas *_texture_atlas);
    void PlayAnim(string _anim_name, int fps);
    void DebugDraw();

  protected:
    map<string, vector<Texture2D>> anim_frame;
    int map_index = 0;
    string last_posible_anim_name = "nan";
    vector<Texture2D> frame_list;

    TextureAtlas *texture_atlas = nullptr;
    int frame_counter = 0;
    int frame_index = 1;
    string anim_name;

    bool _IsPosibleAnimName(string region_map_name, string *posible_anim_name, int *index);
    void _CreateAnimFrame();
    void _OnDraw(int layer_index) override;
  } *atlas_animator;


  /*
    Tilemap Component
  */
  typedef struct Tilemap : BaseTilemap{
    int *tile_map;

    Tilemap(Grid *_grid, int* _tile_map, Texture2D *_texture = nullptr);
    bool IsTiled(int column, int row);
    int* GetTileMap();

  protected:
    void _OnDraw(int layer_index) override;
  } *tilemap;


  /*
    TMXMap Component
  */
  typedef struct TMXMap : BaseTilemap{
    int *layer_sorting_order = nullptr;

    TMXMap(string tmx_file_src, Texture2D *_texture = nullptr);
    void PrintMapAttribute();

    bool IsTiled(int layer_index, int column, int row);
    int GetLayerCount();

  protected:
    int layer_count;
    string map_orientation = "";
    vector<string> map_layer_name_list;
    vector<string> map_layer_str_list;
    vector<int*> map_layer_list;

    void _OnDraw(int layer_index) override;
    void _DrawLayer(int layer_index);
    void _DrawAllLayer();
    void _OnAttach() override;
  } *tmxmap;

}; // component namespace
#endif
