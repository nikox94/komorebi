#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <Object.h>
#include <RGBType.h>
#include <Source.h>
#include <Vect.h>
#include <Ray.h>
#include <Camera.h>
#include <Color.h>
#include <Light.h>
#include <Sphere.h>
#include <Plane.h>
#include <Triangle.h>

#if defined __linux__ || defined __APPLE__
// Compiled for Linux
#else
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

using namespace std;

struct IndexedDouble {
    double value;
    int index;
};

// These are external variables to be used in the program
static int WIDTH = 640, HEIGHT = 480, DPI = 72;
// Recursion depth
static int MAXDEPTH = 5;
// Anti-aliasing depth
static int AADEPTH = 1;
// The machine-precision accuracy (for comparing doubles)
static double ACCURACY = 1E-6;
// Vectors that define the camera
static Vect LOOKFROM, LOOKAT, UP;
// Output file's name
static string OUTFILE;
// The camera object
static Camera SCENE_CAM;
// This tracks the currently active color within the inputfile
// This is how different colors are assigned to objects
static Color CURRENT_COLOR;
// This tracks the ambient light value for the current scene
static double AMBIENTLIGHT = 0.2;

void savebmp (const char *filename, int w, int h, RGBType *data) {
    FILE *f;
    int k = w*h;
    int s = 4*k;
    int filesize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = DPI*m;

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize>>8);
    bmpfileheader[4] = (unsigned char)(filesize>>16);
    bmpfileheader[5] = (unsigned char)(filesize>>24);

    bmpinfoheader[4] = (unsigned char)(w);
    bmpinfoheader[5] = (unsigned char)(w>>8);
    bmpinfoheader[6] = (unsigned char)(w>>16);
    bmpinfoheader[7] = (unsigned char)(w>>24);

    bmpinfoheader[8] = (unsigned char)(h);
    bmpinfoheader[9] = (unsigned char)(h>>8);
    bmpinfoheader[10] = (unsigned char)(h>>16);
    bmpinfoheader[11] = (unsigned char)(h>>24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s>>8);
    bmpinfoheader[23] = (unsigned char)(s>>16);
    bmpinfoheader[24] = (unsigned char)(s>>24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm>>8);
    bmpinfoheader[27] = (unsigned char)(ppm>>16);
    bmpinfoheader[28] = (unsigned char)(ppm>>24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm>>8);
    bmpinfoheader[31] = (unsigned char)(ppm>>16);
    bmpinfoheader[32] = (unsigned char)(ppm>>24);

    f = fopen(filename, "wb");

    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);

    for (int i = 0; i < k; i++) {
        RGBType rgb = data[i];

        double red = (data[i].r)*255;
        double green = (data[i].g)*255;
        double blue = (data[i].b)*255;

        unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

        fwrite(color, 1, 3, f);
    }

    fclose(f);
}

/**
 * Gets an array of all objects that a ray intersects
 * and finds the winning one's index and distance from the camera.
 * The array of object_intersections should be guaranteed
 * to be a positive distance ( > ACCURACY ) away from the camera.
 */
IndexedDouble winningObjectMultiplier(vector<IndexedDouble> object_intersections) {
    // prevent unnessecary calculations
    if (object_intersections.size() == 0) {
        // if there are no intersections
        IndexedDouble result = IndexedDouble();
        result.index = -1;
        return result;
    }

    if (object_intersections.size() == 1) {
        // if that intersection is greater than zero then it's our index
        return object_intersections.at(0);
    }

    int internal_index_of_min_val = 0;
    double minimum_value = object_intersections.at(0).value;
    for (int i = 1; i < object_intersections.size(); i++) {
        if (object_intersections.at(i).value < minimum_value) {
            minimum_value = object_intersections.at(i).value;
            internal_index_of_min_val = i;
        }
    }

    return object_intersections.at(internal_index_of_min_val);
}

RGBType getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects,
                 int index_of_winning_object, vector<Source*> light_sources, int recursion_depth){
    Object* winning_object = scene_objects.at(index_of_winning_object);
    Vect winning_object_normal = winning_object->getNormalAt(intersection_position);
    Color winning_object_color = winning_object->getColor();

    // Checkerboard pattern
    if (winning_object_color.getSpecial() == 2.0) {
        int square = (int) floor(intersection_position.getVectX()) + (int) floor(intersection_position.getVectZ());

        if ( square % 2 == 0 ) {
            // black tile
            winning_object_color.setRed(0);
            winning_object_color.setGreen(0);
            winning_object_color.setBlue(0);
        }
        else {
            //white tile
            winning_object_color.setRed(1);
            winning_object_color.setGreen(1);
            winning_object_color.setBlue(1);
        }
    }

    RGBType final_color = winning_object_color.getDiffuse() * AMBIENTLIGHT;

    // Reflections
    if (recursion_depth < MAXDEPTH && winning_object_color.getSpecial() > 0 && winning_object_color.getSpecial() <= 1) {
        // reflection from object with specular intensity
        double incidence_angle = winning_object_normal.dot(intersecting_ray_direction.negative());
        Vect scaled_normal = winning_object_normal.mult(incidence_angle);
        Vect add1 = scaled_normal.add(intersecting_ray_direction);
        Vect scaled2 = add1.mult(2);
        Vect add2 = intersecting_ray_direction.negative().add(scaled2);
        Vect reflection_direction = add2.normalise();

        // create reflection ray
        Ray reflection_ray (intersection_position, reflection_direction);

        // determine what the ray intersects with first
        vector<IndexedDouble> reflection_intersections;
        IndexedDouble reflection_multiplier;

        for (int reflection_index = 0; reflection_index < scene_objects.size() ; reflection_index++) {
            reflection_multiplier.value = scene_objects.at(reflection_index)->findIntersection(reflection_ray);
            if (reflection_multiplier.value > ACCURACY) {
                reflection_multiplier.index = reflection_index;
                reflection_intersections.push_back(reflection_multiplier);
            }
        }

        if (reflection_intersections.size() != 0) {
            // reflection ray did intersect

            IndexedDouble winning_object_with_reflection_multiplier = winningObjectMultiplier(reflection_intersections);
            Vect reflection_intersection_position = intersection_position
                                                        .add(reflection_direction
                                                            .mult(winning_object_with_reflection_multiplier.value));
            Vect reflection_intersection_ray_direction = reflection_direction;

            // Recursive call
            RGBType reflection_intersection_color = getColorAt(reflection_intersection_position,
                                                               reflection_intersection_ray_direction,
                                                               scene_objects,
                                                               winning_object_with_reflection_multiplier.index,
                                                               light_sources,
                                                               recursion_depth + 1
                                                              );
            final_color += reflection_intersection_color * winning_object_color.getSpecial();
        }
    // End of reflections
    }

    // Process scene light
    for (int light_index = 0; light_index < light_sources.size() ; light_index++) {
        Source* light = light_sources.at(light_index);
        Vect light_direction = light->getPosition().add(intersection_position.negative()).normalise();

        float cosine_angle = winning_object_normal.dot(light_direction);

        if (cosine_angle > 0) {
            // test for shadows
            bool shadowed = false;

            Vect distance_to_light = light->getPosition().add(intersection_position.negative()).normalise();
            float distance_to_light_magnitude = distance_to_light.magnitude();

            Ray shadow_ray (intersection_position, light->getPosition().add(intersection_position.negative()).normalise());

            vector<double> secondary_intersections;

            for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) {
                secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
            }

            for (int c = 0; c < secondary_intersections.size(); c++) {
                if (secondary_intersections.at(c) > ACCURACY) {
                    if (secondary_intersections.at(c) <= distance_to_light_magnitude) {
                        shadowed = true;
                    }
                    break;
                }
            }

            if (shadowed == false) {
                final_color = final_color + winning_object_color.getDiffuse() * (light_sources.at(light_index)->getColor().getDiffuse() * cosine_angle);

                if (winning_object_color.getSpecial() > 0 && winning_object_color.getSpecial() <= 1) {
                    double dot1 = winning_object_normal.dot(intersecting_ray_direction.negative());
                    Vect scalar1 = winning_object_normal.mult(dot1);
                    Vect add1 = scalar1.add(intersecting_ray_direction);
                    Vect scalar2 = add1.mult(2);
                    Vect add2 = intersecting_ray_direction.negative().add(scalar2);
                    Vect reflection_direction = add2.normalise();

                    double specular = reflection_direction.dot(light_direction);
                    if (specular > 0) {
                        specular = pow(specular, 10);
                        final_color = final_color + (light_sources.at(light_index)->getColor().getDiffuse() * (specular*winning_object_color.getSpecial()));
                    }
                }
            }
        }
    // End of processing scene light
    }

    return final_color.clip();
}

RGBType* raytrace (vector<Source*> light_sources, vector<Object*> scene_objects) {
    int n = WIDTH*HEIGHT;
    double aspectratio = (double) WIDTH / (double) HEIGHT;


    RGBType *pixels = new RGBType[n];
    int thisone, aa_index;
    double xamnt, yamnt;
    // Anti-aliasing
    // Start with blank pixels
    double tempRed[AADEPTH*AADEPTH];
    double tempGreen[AADEPTH*AADEPTH];
    double tempBlue[AADEPTH*AADEPTH];

    // pixel main loop
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            thisone = y*WIDTH + x;

            // Anti-aliasing loop
            for (int aax = 0; aax < AADEPTH; aax++) {
                for (int aay = 0; aay < AADEPTH; aay++) {
                    aa_index = aay*AADEPTH + aax;
                    srand(time(0));

                    // Create the ray from the camera to this pixel

                    double offset = (AADEPTH == 1 ? 0.5 : (double) aax/((double) AADEPTH - 1));
                    // Anti-aliasing
                    if (WIDTH > HEIGHT) {
                        // the image is wider than it is tall
                        xamnt = (((x + offset)/WIDTH)*aspectratio - ((WIDTH-HEIGHT)/ (double) HEIGHT));
                        yamnt = ((HEIGHT - y) + offset)/HEIGHT;
                    } else if (HEIGHT > WIDTH) {
                        // the image is taller than it is wide
                        xamnt = (x+offset)/WIDTH;
                        yamnt = (((HEIGHT - y) + offset)/HEIGHT)/aspectratio - ((HEIGHT - WIDTH)/(double) WIDTH);
                    } else {
                        // the image is square
                        xamnt = (x+offset)/WIDTH;
                        yamnt = ((HEIGHT - y) + offset)/HEIGHT;
                    }

                    Vect cam_ray_origin = SCENE_CAM.getCameraPosition();
                    Vect cam_ray_direction = SCENE_CAM.getCameraDirection()
                                                        .add(SCENE_CAM.getCameraRight().mult(xamnt - 0.5)
                                                        .add(SCENE_CAM.getCameraDown().mult(yamnt - 0.5)))
                                                        .normalise();
                    Ray cam_ray (cam_ray_origin, cam_ray_direction);

                    // Calculate all object intersections with cam_ray
                    vector<IndexedDouble> intersections;
                    IndexedDouble intersection_multiplier;

                    for (int index = 0; index < scene_objects.size(); index++) {
                        intersection_multiplier.value = scene_objects.at(index)->findIntersection(cam_ray);
                        if (intersection_multiplier.value > ACCURACY) {
                            intersection_multiplier.index = index;
                            intersections.push_back(intersection_multiplier);
                        }
                    }

                    if(intersections.size() == 0) {
                        tempRed[aa_index] = 0.0;
                        tempGreen[aa_index] = 0.0;
                        tempBlue[aa_index] = 0.0;
                    } else {
                        IndexedDouble winning_object_multiplier = winningObjectMultiplier(intersections);

                        // determine the position and direction vectors at intersection
                        Vect intersection_position = cam_ray_origin.add(cam_ray_direction.mult(winning_object_multiplier.value));
                        Vect intersecting_ray_direction = cam_ray_direction;

                        RGBType current_obj_color = getColorAt(intersection_position,
                                                               intersecting_ray_direction,
                                                               scene_objects,
                                                               winning_object_multiplier.index,
                                                               light_sources,
                                                               0);
                        tempRed[aa_index] = current_obj_color.r;
                        tempGreen[aa_index] = current_obj_color.g;
                        tempBlue[aa_index] = current_obj_color.b;
                    }
                // End of Anti-aliasing loop
                }
            }

            // average the pixel color
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;

            for (int iRed = 0; iRed < AADEPTH*AADEPTH; iRed++) {
                totalRed += tempRed[iRed];
            }

            for (int iGreen = 0; iGreen < AADEPTH*AADEPTH; iGreen++) {
                totalGreen += tempGreen[iGreen];
            }

            for (int iBlue = 0; iBlue < AADEPTH*AADEPTH; iBlue++) {
                totalBlue += tempBlue[iBlue];
            }

            double avgRed = totalRed/(AADEPTH*AADEPTH);
            double avgGreen = totalGreen/(AADEPTH*AADEPTH);
            double avgBlue = totalBlue/(AADEPTH*AADEPTH);

            pixels[thisone].r = avgRed;
            pixels[thisone].g = avgGreen;
            pixels[thisone].b = avgBlue;
            // end of pixel loop
        }
    }
    return pixels;
}

void readSceneFile(int argc, char* argv[], vector<Source*> *light_sources,
                   vector<Object*> *scene_objects, vector<Vect*> *vertices) {
    // Read input file with instructions
    if (argc != 2) {
        cout << "Please specify the scene file to read!" << endl;
        exit(1);
    }

    ifstream sceneFile;
    sceneFile.open(argv[1]);

    if (!sceneFile.is_open()) {
        cout << "Could not open file " << argv[1] << endl;
        exit(1);
    }

    string line;
    while ( getline (sceneFile, line) ) {
        if ( line[0] == '#' || line[0] == '\0') continue;

        stringstream ss (stringstream::out | stringstream::in);
        ss.str(line);
        string op;
        ss >> op;

        // GENERAL SCENE
        if ( op.compare("size") == 0) {
            ss >> WIDTH >> HEIGHT;
            continue;
        }
        if ( op.compare("dpi") == 0) {
            ss >> DPI;
            continue;
        }
        if ( op.compare("output") == 0) {
            ss >> OUTFILE;
            continue;
        }
        if ( op.compare("maxdepth") == 0) {
            ss >> MAXDEPTH;
            continue;
        }
        if ( op.compare("aadepth") == 0) {
            ss >> AADEPTH;
            continue;
        }
        if ( op.compare("accuracy") == 0) {
            ss >> ACCURACY;
            continue;
        }
        if ( op.compare("camera") == 0 ) {
            double x, y, z;
            ss >> x >> y >> z;
            LOOKFROM = Vect(x, y, z);
            ss >> x >> y >> z;
            LOOKAT = Vect(x, y, z);
            ss >> x >> y >> z;
            UP = Vect(x, y, z);

            Vect camdir = LOOKFROM.negative()
                                  .add(LOOKAT)
                                  .normalise();
            Vect camright = UP.cross(camdir).normalise();
            Vect camdown = camright.cross(camdir);
            SCENE_CAM = Camera(LOOKFROM, camdir, camright, camdown);
            continue;
        }
        // END GENERAL SCENE

        // LIGHTS
        if ( op.compare("point") == 0 ) {
            double x, y, z, r, g, b;
            ss >> x >> y >> z >> r >> g >> b;
            Color color = Color(r, g, b, 0.0);
            Vect position = Vect(x, y, z);
            Light* light = new Light(position, color);
            light_sources->push_back(dynamic_cast<Source*>(light));
            continue;
        }
        if ( op.compare("ambient") == 0 ) {
            RGBType ambient;
            ss >> ambient.r >> ambient.g >> ambient.b;
            continue;
        }
        // END LIGHTS

        // SCENE OBJECTS
        if ( op.compare("sphere") == 0 ) {
            double x, y, z, radius;
            ss >> x >> y >> z >> radius;
            Sphere* sphere = new Sphere( Vect(x, y, z),
                                         radius,
                                         CURRENT_COLOR);
            scene_objects->push_back(dynamic_cast<Object*>(sphere));
            continue;
        }
        if ( op.compare("vertex") == 0 ) {
            double x, y, z;
            ss >> x >> y >> z;
            Vect* vect = new Vect(x, y, z);
            vertices->push_back(vect);
            continue;
        }
        if ( op.compare("tri") == 0 ) {
            int v1, v2, v3;
            ss >> v1 >> v2 >> v3;
            Triangle* tri = new Triangle(*vertices->at(v1),
                                         *vertices->at(v2),
                                         *vertices->at(v3),
                                         CURRENT_COLOR);
            scene_objects->push_back(dynamic_cast<Object*>(tri));
            continue;
        }
        if ( op.compare("plane") == 0 ) {
            int nx, ny, nz, dist;
            ss >> nx >> ny >> nz >> dist;
            Plane* plane = new Plane(Vect(nx, ny, nz), dist, CURRENT_COLOR);
            scene_objects->push_back(dynamic_cast<Object*>(plane));
            continue;
        }
        // END SCENE OBJECTS

        // COLOUR
        if ( op.compare("diffuse") == 0 ) {
            RGBType diffuse;
            ss >> diffuse.r >> diffuse.g >> diffuse.b;
            CURRENT_COLOR.setRed(diffuse.r);
            CURRENT_COLOR.setGreen(diffuse.g);
            CURRENT_COLOR.setBlue(diffuse.b);
            continue;
        }
        if ( op.compare("shininess") == 0 ) {
            double shine;
            ss >> shine;
            CURRENT_COLOR.setShine(shine);
            continue;
        }
        if ( op.compare("special") == 0 ) {
            double special;
            ss >> special;
            CURRENT_COLOR.setSpecial(special);
            continue;
        }
        // END COLOUR
    }

    sceneFile.close();
}


int main (int argc, char *argv[]) {
    cout << "reading input file..." << endl;
    vector<Source*> light_sources;
    vector<Object*> scene_objects;
    vector<Vect*> vertices;
    readSceneFile(argc, argv, &light_sources, &scene_objects, &vertices);

    cout << "rendering..." << endl;

    // Measure the time of execution of the rendering
    clock_t t1, t2;
    t1 = clock();

    RGBType *pixels = raytrace(light_sources, scene_objects);
    savebmp(OUTFILE.c_str(), WIDTH, HEIGHT, pixels);

    delete pixels;

    // Calculate the time for rendering
    t2 = clock();
    float diff = ( (float) t2 - (float) t1) / CLOCKS_PER_SEC;
    cout << "This rendering took "<< diff << "seconds.\n";

    return 0;
}
