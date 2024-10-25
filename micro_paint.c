/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eschmitz <eschmitz@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:40:45 by eschmitz          #+#    #+#             */
/*   Updated: 2024/10/02 16:35:35 by eschmitz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_data
{
    int     width;
    int     height;
    char    background;
    char    *matrice;
}   t_data;

typedef struct s_rectangle
{
    char    mode;
    float   x;
    float   y;
    float   width;
    float   height;
    char    colour;
}   t_rectangle;

int is_colour(float x, float y, t_rectangle rectangle)
{   
    if (x < rectangle.x || rectangle.x + rectangle.width < x || y < rectangle.y || rectangle.y + rectangle.height < y)
        return (0);
    if (rectangle.mode == 'R')
        return (1);
    if (x < rectangle.x + 1.00000000 || x > rectangle.x + rectangle.width - 1.00000000 || y < rectangle.y + 1.00000000 || y > rectangle.y + rectangle.height - 1.00000000)
        return (1);
    return (0);    
}

int initialise_data(t_data *data, FILE *file)
{
    if (fscanf(file, "%d %d %c\n", &data->width, &data->height, &data->background) != 3)
        return (1);
    if (data->width <= 0 || data->width > 300 || data->height <= 0 || data->height > 300)
        return (1);
    data->matrice = malloc(sizeof(char) * (data->height * data->width));
    if (!data->matrice)
        return (1);
    memset(data->matrice, data->background, data->width * data->height);
    return (0);
}

int process(t_data *data, FILE *file)
{
    t_rectangle    rectangle;
    int         y;
    int         x;

    while (fscanf(file, "%c %f %f %f %f %c\n", &rectangle.mode, &rectangle.x, &rectangle.y, &rectangle.width, &rectangle.height, &rectangle.colour) == 6)
    {
        if ((rectangle.mode != 'r' && rectangle.mode != 'R') || rectangle.width <= 0.00000000 || rectangle.height <= 0.00000000)
            return (1);
        x = -1;
        while (++x < data->width)
        {
            y = -1;
            while (++y < data->height)
                if (is_colour((float)x, (float)y, rectangle))
                    data->matrice[y * data->width + x] = rectangle.colour;
        }
    }
    return (0);
}

int main(int argc, char **argv)
{
    FILE    *file;
    t_data  data;
    int     y;

    if (argc != 2)
        return (write(1, "Error: argument\n", 16), 1);
    if (!(file = fopen(argv[1], "r")) || initialise_data(&data, file))
        return (write(1, "Error: Operation file corrupted\n", 32), 1);
    if (process(&data, file))
    {
        free(data.matrice);
        fclose(file);
        return (write(1, "Error: Operation file corrupted\n", 32), 1);
    }
    y = -1;
    while (++y < data.height)
    {
        write(1, y * data.width + data.matrice, data.width);
        write(1, "\n", 1);
    }
    return (free(data.matrice), fclose(file), 0);
}
