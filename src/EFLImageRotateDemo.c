/*
 * Author: Joseph Yang
 * Date:   2011/02/11
 *
 *
 * COPYRIGHT(R) Reserved by AVerMedia Co. Ltd.
 * */

#include <Ecore_Evas.h>
#include <Ecore.h>

#define WIDTH         1920
#define HEIGHT        1080
#define FRAME_RATE    (1.0 / 30.0) /* animation rendering frame rate, default is 1/30 seconds */
#define ROTATE_ANGLE  30.0 /* rotate angle per frame, default is 30.0 degrees */

Ecore_Evas     * ee;
Evas           * evas;
Evas_Object    * img;

Ecore_Animator * animator;
double           angle = 0;

int              img_width;
int              img_height;

static Eina_Bool rotate_animation(void *data)
{
	Evas_Map *map = NULL;
	int W = WIDTH;
	int H = HEIGHT;
	Evas_Coord x, y ,z, w, h;
	int idx = 0;

	angle = (double) ((int) (angle + ROTATE_ANGLE) % 360);

	evas_object_geometry_get(img, &x, &y, &w, &h);
	map = evas_map_dup(evas_object_map_get(img));

	if (map == NULL)
	{
		map = evas_map_new(4);
	}

	evas_map_util_points_populate_from_geometry(map, x, y, w, h, 0);

	evas_map_util_rotate(map, angle, W / 2, H / 2);
	evas_map_alpha_set(map, 1);
	evas_map_smooth_set(map, 1);
	evas_object_map_enable_set(img, 1);
	evas_object_map_set(img, map);

	evas_map_free(map);

	return EINA_TRUE;
}

void mouse_callback(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Event_Mouse_Down * mouse = (Evas_Event_Mouse_Down *) event_info;

	if (mouse->button == 1)
	{
		//rotate_animation(NULL);
	}
	else
	{
		ecore_main_loop_quit();
	}
}

int main(void)
{
	Evas_Object * bg = NULL;

	ecore_init();
	ecore_evas_init();

	/* Get the first available rendering engine from the candidate list, please specify the engine name if this doesn't work */
	ee = ecore_evas_new(NULL, 0, 0, WIDTH, HEIGHT, NULL);
	ecore_evas_title_set(ee, "Ecore_Evas Template");
	ecore_evas_borderless_set(ee, 0);

	printf("\n----------- Ecore evas engine is: %s -----------------\n", ecore_evas_engine_name_get(ee));

	/* Full screen mode, remove the comment to enable */
	//ecore_evas_fullscreen_set(ee, EINA_TRUE);
	ecore_evas_show(ee);
	evas = ecore_evas_get(ee);

	bg = evas_object_rectangle_add(evas);
	evas_object_resize(bg, WIDTH, HEIGHT);
	evas_object_color_set(bg, 255,120,0,255);
	evas_object_move(bg, 0, 0);
	evas_object_show(bg);

	/* Image Object Initialization */
	img = evas_object_image_add(evas);
	evas_object_image_file_set(img, "e.png", NULL);
	evas_object_image_size_get(img, &img_width, &img_height);
	evas_object_resize(img, img_width, img_height);
	evas_object_image_fill_set(img, 0 ,0, img_width, img_height);

	/* move the object to center position*/
	evas_object_move(img, (WIDTH - img_width) / 2, (HEIGHT - img_height) / 2);

	evas_object_show(img);

	/* animation setup*/
	rotate_animation(NULL);
	animator = ecore_animator_add(&rotate_animation, NULL);

	/* frame rate, change the frame rate*/
	ecore_animator_frametime_set(FRAME_RATE);

	evas_object_event_callback_add(img, EVAS_CALLBACK_MOUSE_DOWN, &mouse_callback, NULL);
	ecore_main_loop_begin();

	evas_object_event_callback_del(img, EVAS_CALLBACK_MOUSE_DOWN, &mouse_callback);

	ecore_evas_shutdown();
	ecore_shutdown();
	return 0;
}

