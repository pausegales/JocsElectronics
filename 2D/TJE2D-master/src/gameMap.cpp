#include "gameMap.h"
#include "json.hpp"
#include <fstream>

GameMap::GameMap() {
    width = height = 0;
    data = nullptr;
    layers = nullptr;
}

GameMap::GameMap(int w, int h) {
    width = w;
    height = h;
    data = new sCell[w * h];
}

sCell& GameMap::getCell(int x, int y) {
    return data[x + y * width];
}

sCell& GameMap::getCell(int x, int y, int l) {
    return layers[l].data[x + y * width];
}

void GameMap::drawMap(Image* framebuffer, int layer, float time, Image tileset, Camera camera) {
    // Size in pixels of a cell
    const int numtiles = 32;
    int cs = tileset.width / numtiles;

    // For every cell
    for (int x = 0; x < width; ++x)
        for (int y = 0; y < height; ++y)
        {
            // Get cell info
            sCell& cell = getCell(x, y, layer);
            if (cell.type == -1)
                continue;

            int type = (int)cell.type;
            // Animate in case of animated type
            if (type == TORCH_OFF) {
                type = TORCH_OFF + (int(time * 4)) % 8;
            }
            if (type == FLAME_OFF) {
                type = FLAME_OFF + (int(time * 4)) % 8;
            }
            if (type == FIRE2) {
                type = ( ((int(time * 3.5)) % 2) == 1 ? (FIRE1) : FIRE2);
            }

            // Compute tile pos in tileset image
            int tilex = (type % numtiles) * cs;
            int tiley = floor(type / numtiles) * cs;

            // Create tile area
            Area area(tilex, tiley, cs, cs);

            // Place offset here if you want
            int screenx = x * cs - camera.offset.x;
            int screeny = y * cs - camera.offset.y;



            // Avoid rendering out of screen stuff
            if (screenx < -cs || screenx >(int)framebuffer->width ||
                screeny < -cs || screeny >(int)framebuffer->height)
                continue;

            // Draw region of tileset inside framebuffer
            framebuffer->drawImage(tileset, screenx, screeny, area);
        }
}

//
//void GameMap::drawMapPlayer(Image* framebuffer, float time, Image tileset, Camera camera, Vector2 position) {
//    // Size in pixels of a cell
//    const int numtiles = 32;
//    int cs = tileset.width / numtiles;
//
//    int x1 = (position.x - 16 / 2) / 16 + 1; //converteix player position a quina cel·la
//    int y1 = (position.y - 16 / 3.9) / 16 + 1;
//
//    // For every cell
//    int idx = HOLE1;
//    for (int y = 0; y < height; ++y) {
//        for (int x = 0; x < width; ++x)
//        {
//            // Get cell info
//            sCell& cell = getCell(x, y, 0);
//            if (cell.type == -1)
//                continue;
//
//            int type = (int)cell.type;
//            if ((x1 - 2 < x && x < x1 + 2) && (y1 - 2 < y && y < y1 + 2)) { type = idx; idx++; }
//
//            // Compute tile pos in tileset image
//            int tilex = (type % numtiles) * cs;
//            int tiley = floor(type / numtiles) * cs;
//
//            // Create tile area
//            Area area(tilex, tiley, cs, cs);
//
//            // Place offset here if you want
//            int screenx = x * cs - camera.offset.x;
//            int screeny = y * cs - camera.offset.y;
//
//            // Avoid rendering out of screen stuff
//            if (screenx < -cs || screenx >(int)framebuffer->width ||
//                screeny < -cs || screeny >(int)framebuffer->height)
//                continue;
//            
//            // Draw region of tileset inside framebuffer
//            framebuffer->drawImage(tileset, screenx, screeny, area);
//        }
//    }
//}




void GameMap::loadGameMap(const char* filename)
{
    using json = nlohmann::json;
    std::ifstream f(filename);
    if (!f.good())
        return;
    json jData = json::parse(f);

    width = jData["width"];
    height = jData["height"];

    int numLayers = jData["layers"].size();
    //int numLayers = 2;
    layers = new sLayer[numLayers];

    for (int l = 0; l < numLayers; ++l) {
        layers[l].data = new sCell[width * height];
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int index = x + y * width;
                int type = jData["layers"][l]["data"][index].get<int>();
                sCell& cell = getCell(x, y, l);
                cell.type = eCellType(type - 1);
            }
        }
    }
    f.close();
}


bool GameMap::isValid(Vector2 pos, int size)
{
    
    eCellType& type = getCell((pos.x - size/1)/16+1, (pos.y - size/3.7)/16+1.3, 6).type;
    //2printf("%f", type);
    if ((type == COLLISION)||(type == SPECIAL_COLLISION)) {
        return false;
    }
    return true;
}
