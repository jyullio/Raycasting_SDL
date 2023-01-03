#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WIN_WIDTH 1024
#define WIN_HEIGHT 512

#define MARK_POINT_SIZE 8

#define ANGLE_BETEEN_RAYS (0.0872665)
#define QTD_RAYS_OF_CURSOR 72
#define INFINITE_LEN 1000

#define QTD_WALLS 7

struct point2D
{
    int x, y;
};

struct line2D
{
    struct point2D p1 , p2;

};

struct raycasting
{
    struct line2D* rays_of_cursor;
    struct point2D* points_of_collision;
};




struct line2D walls[QTD_WALLS];
int global_mouse_x;
int global_mouse_y;



bool get_line_intersection(struct line2D* line1, struct line2D* line2 , struct point2D* point_of_colision)
{
    
    float x1 = line1 -> p1.x , y1 = line1 -> p1.y;
    float x2 = line1 -> p2.x , y2 = line1 -> p2.y;
    float x3 = line2 -> p1.x , y3 = line2 -> p1.y;
    float x4 = line2 -> p2.x , y4 = line2 -> p2.y;

    float denominator = (x2 - x1) * (y4 - y3) - (y2 - y1) * (x4 - x3);

    float aux_x = x3 - x1;
    float aux_y = y3 - y1;

    float t = ((aux_x) * (y4 - y3) - (aux_y) * (x4 - x3)) / denominator;
    float u = ((aux_x) * (y2 - y1) - (aux_y) * (x2 - x1)) / denominator;
    
    if((0 <= t && t <= 1) && (0 <= u && u <= 1))
    {
        //printf("\n\nIntersection\n\n");
        point_of_colision -> x = x1 + t * (x2 - x1);
        point_of_colision -> y = y1 + t * (y2 - y1);
        return true;
    }

    
    return false;
   
}

double dist_of_points(struct point2D* point1 , struct point2D* point2)
{
    return sqrt((point1 -> x - point2 -> x) * (point1 -> x - point2 -> x) + (point1 -> y - point2 -> y) * (point1 -> y - point2 -> y));
}

void process_cursor_raycasting(struct raycasting* raycasting)
{
    struct point2D mouse;
    SDL_GetMouseState(&mouse.x, &mouse.y);

    double angle = 0;
    for(int i = 0 ; i < QTD_RAYS_OF_CURSOR ; i++ , angle += ANGLE_BETEEN_RAYS)
    {
        raycasting -> rays_of_cursor[i].p1.x = mouse.x;
        raycasting -> rays_of_cursor[i].p1.y = mouse.y;
        raycasting -> rays_of_cursor[i].p2.x = mouse.x + (INFINITE_LEN * cos(angle));
        raycasting -> rays_of_cursor[i].p2.y = mouse.y + (INFINITE_LEN * sin(angle));

    }


    for(int i = 0 ; i < QTD_RAYS_OF_CURSOR ; i++)
    {
        //get all points of intersection of ray
        int qtd_intersections = 0;
        for(int j  = 0 ; j < QTD_RAYS_OF_CURSOR ; j++)
        {
            
            if(get_line_intersection(&(raycasting -> rays_of_cursor[i]) , &walls[j] , &(raycasting -> points_of_collision[qtd_intersections])))
            {
               qtd_intersections++;
            }     
        
        }

        if(qtd_intersections == 0)
            continue;

        //find the closest ray intersection point with mouse position
        int closest_intersection_point = 0;
        double shorter_dist = dist_of_points(&(raycasting -> points_of_collision[closest_intersection_point]) , &mouse);
        for(int j = 1 ; j < qtd_intersections ; j++)
        {
            double cur_dist = dist_of_points(&(raycasting -> points_of_collision[j]) , &mouse);
            if(shorter_dist > cur_dist)
            {
                shorter_dist = cur_dist;
                closest_intersection_point = j;
            }
        }

        //set the new limit of the ray(it's the closest intersection point)
        raycasting -> rays_of_cursor[i].p2.x = raycasting -> points_of_collision[closest_intersection_point].x;
        raycasting -> rays_of_cursor[i].p2.y = raycasting -> points_of_collision[closest_intersection_point].y;
        
        



    }
    

}


void render(SDL_Renderer* renderer , struct raycasting* raycasting)
{


    //clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    /*
    //render point at mouse position
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    //printf("\nx: %i --- y%i\n" , mouse_x , mouse_y);
    SDL_Rect mouse_rect;
    mouse_rect.x = mouse_x - (MARK_POINT_SIZE / 2);
    mouse_rect.y = mouse_y - (MARK_POINT_SIZE / 2);
    mouse_rect.w = MARK_POINT_SIZE;
    mouse_rect.h = MARK_POINT_SIZE;
    SDL_RenderFillRect(renderer, &mouse_rect);
    */

    //draw rays of cursor
    
    for(int i = 0 ; i < QTD_RAYS_OF_CURSOR ; i++)
    {
        SDL_RenderDrawLine(renderer, raycasting -> rays_of_cursor[i].p1.x, raycasting -> rays_of_cursor[i].p1.y, raycasting -> rays_of_cursor[i].p2.x , raycasting -> rays_of_cursor[i].p2.y);
    }
    
    
    //render walls
    for(int i = 0 ; i < QTD_WALLS ; i++)
    {
         SDL_RenderDrawLine(renderer, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x , walls[i].p2.y);
    }
    

    /*
    //render blue points in colisions points
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for(int i = 0 ; i < QTD_RAYS_OF_CURSOR ; i++)
    {
        SDL_Rect mark_point_rect;
        mark_point_rect.x = rays_of_cursor[i].p2.x - (MARK_POINT_SIZE / 2);
        mark_point_rect.y = rays_of_cursor[i].p2.y - (MARK_POINT_SIZE / 2);
        mark_point_rect.w = MARK_POINT_SIZE;
        mark_point_rect.h = MARK_POINT_SIZE;
        SDL_RenderFillRect(renderer, &mark_point_rect);
    }
    */

    SDL_RenderPresent(renderer);


}


void init_walls(void)
{

    walls[4].p1.x = 700;
    walls[4].p1.y = 100;
    walls[4].p2.x = 700;
    walls[4].p2.y = 300;

    walls[5].p1.x = 403;
    walls[5].p1.y = 28;
    walls[5].p2.x = 148;
    walls[5].p2.y = 488;
    
    walls[0].p1.x = 271;
    walls[0].p1.y = 65;
    walls[0].p2.x = 450;
    walls[0].p2.y = 346;


    walls[1].p1.x = 800;
    walls[1].p1.y = 100;
    walls[1].p2.x = 800;
    walls[1].p2.y = 300;

    
    walls[2].p1.x = 183;
    walls[2].p1.y = 125;
    walls[2].p2.x = 684;
    walls[2].p2.y = 144;

    walls[3].p1.x = 118;
    walls[3].p1.y = 372;
    walls[3].p2.x = 465;
    walls[3].p2.y = 492;

    walls[6].p1.x = 844;
    walls[6].p1.y = 183;
    walls[6].p2.x = 618;
    walls[6].p2.y = 274;
    
    

}


void init_raycasting(struct raycasting* raycasting)
{
    raycasting -> rays_of_cursor = (struct line2D*)malloc(sizeof(struct line2D) * QTD_RAYS_OF_CURSOR);
    raycasting -> points_of_collision = (struct point2D*)malloc(sizeof(struct point2D) * QTD_WALLS);
}

int main(int argc, char** argv)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error init SDL!!!");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIN_WIDTH, WIN_HEIGHT,
                                          0);



    if(!window)
    {
        printf("Error init window!!!");
        return -1;
    }


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );

    if(!renderer)
    {
        printf("Error init render");
        return -1;
    }

    struct raycasting raycasting;
    init_walls();
    init_raycasting(&raycasting);
    process_cursor_raycasting(&raycasting);
    //SDL_GetMouseState(&global_mouse_x, &global_mouse_y);
    


   
    

    SDL_Event e;
    bool running = true;
    while(running)
    {
        while ( SDL_PollEvent( &e ) != 0 ) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    break;

            }
        }

        process_cursor_raycasting(&raycasting);
        render(renderer , &raycasting);

    }

    SDL_Quit();

    return 0;
}