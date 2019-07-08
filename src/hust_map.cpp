#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#ifdef __linux__
#include <GL/glut.h>
#else
#include <glut.h>
#endif

GLuint textures[29];

int jet_enable = 1;

float blob_x = -50;
float jet_speed = 1.31;
float zoom = 1;
float eye_x = 4000;
float eye_y = 150;
float eye_z = 15000;
float centre_x = 4000;
float centre_y = 150;
float centre_z = 14000;
float up_x = 0;
float up_y = 1;
float up_z = 0;
float eye_step = 30;
float angle_xz = 0;
float angle_y = 0;
float angle_step = M_PI / (120 - eye_step);

GLuint load_texture(std::string file_name, int width, int height);
void get_textures();
void initialise();
void display();
void spin_display();
void timer(int n);
void reshape(int width, int height);
void special(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

void draw_ground();
void draw_building();
void draw_building_ground();
void draw_building_trees();
void draw_building_floor();
void draw_hall_floor();
void draw_rooms();
void draw_back_porch();
void draw_vestibule();
void draw_vestibule_panes();
void draw_vestibule_pillars();
void draw_stair_space();
void draw_building_ceiling();

void up_to_down_square_face(float x, float z, float x_repeat, float z_repeat, int tex);
void down_to_up_square_face(float x, float z, float x_repeat, float z_repeat, int tex);
void front_to_back_square_face(float x, float y, float x_repeat, float y_repeat, int tex);
void back_to_front_square_face(float x, float y, float x_repeat, float y_repeat, int tex);
void right_to_left_square_face(float y, float z, float y_repeat, float z_repeat, int tex);
void left_to_right_square_face(float y, float z, float y_repeat, float z_repeat, int tex);
void inner_box(float x, float y, float z, float x_repeat, float y_repeat, float z_repeat, int left, int right, int back, int front, int under, int upper);
void outer_box(float x, float y, float z, float x_repeat, float y_repeat, float z_repeat, int left, int right, int back, int front, int under, int upper);
void vertical_cylinder(float base_radius, float top_radius, float height, float xz_repeat, float y_repeat, int xz_split, int y_split, int tex);
void vertical_sphere(float radius, float xz_repeat, float y_repeat, int xz_split, int y_split, int tex);
void horizonal_disk(float inner_radius, float outer_radius, float x_repeat, float z_repeat, int xz_split, int y_split, int tex);
void partial_horizonal_disk(float inner_radius, float outer_radius, float start_angle, float sweep_angle, float x_repeat, float z_repeat, int xz_split, int y_split, int tex);

void room_frame();
void eaves();
void corridor();
void small_pillar();
void big_pillar();
void room_pillars();
void left_front_handrail();
void right_front_handrail();
void edge_handrail();
void window();
void door();
void room(std::string type);
void covered();
void back_porch_pillars();

void centre_ground();
void centre_grass();
void stone();
void flags();
void centre_small_trees();
void centre_big_trees();
void centre_trees();
void dam();
void water_spout();
void jet();
void jets();
void centre();

void flag();
void small_tree(float base_radius, float top_radius, float height, float radius, float rind_repeat, float xz_leaves_repeat, float y_leaves_repeat);
void big_tree(float base_radius, float rind_height, float rind_repeat, float root_height, float radius, int canopy, float xz_leaves_repeat, float y_leaves_repeat, int tex);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1360, 700);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("hust_map");
    get_textures();
    initialise();
    glutDisplayFunc(display);
    timer(5);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

GLuint load_texture(std::string file_name, int width, int height) {
    GLuint texture;
    unsigned char* data;
    FILE* file = fopen(file_name.c_str(), "rb");

    if (file == NULL) {
        printf("File Not Found : %s.\n", file_name.c_str());
        return 0;
    }

    fseek(file, 54, SEEK_CUR);
    data = (unsigned char*)malloc(width * height * 3);
    if (fread(data, width * height * 3, 1, file) != 1) {
        printf("Faied to read data.\n");
    }
    fclose(file);

    for (int i = 0; i < width * height; ++i) {
        int index = i * 3;
        unsigned char b, r;
        b = data[index];
        r = data[index + 2];
        data[index] = r;
        data[index + 2] = b;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glEnable(GL_TEXTURE_2D);
    free(data);
    return texture;
}

void get_textures() {
    textures[0] = load_texture("textures/grass2.bmp", 1296, 1296);
    textures[1] = load_texture("textures/dam.bmp", 479, 144);
    textures[2] = load_texture("textures/leaves.bmp", 500, 369);
    textures[3] = load_texture("textures/rind.bmp", 259, 194);
    textures[4] = load_texture("textures/dambed.bmp", 706, 557);
    textures[5] = load_texture("textures/flag.bmp", 1024, 768);
    textures[6] = load_texture("textures/lamp.bmp", 200, 200);
    textures[7] = load_texture("textures/stair.bmp", 472, 469);
    textures[8] = load_texture("textures/roadbed.bmp", 640, 480);
    textures[9] = load_texture("textures/lampbed.bmp", 200, 200);
    textures[10] = load_texture("textures/stone2.bmp", 503, 334);
    textures[11] = load_texture("textures/stone_.bmp", 500, 502);
    textures[12] = load_texture("textures/brick.bmp", 604, 606);
    textures[13] = load_texture("textures/brick2.bmp", 488, 490);
    textures[14] = load_texture("textures/waterdam.bmp", 256, 256);
    textures[15] = load_texture("textures/blob.bmp", 69, 69);
    textures[16] = load_texture("textures/window.bmp", 520, 319);
    textures[17] = load_texture("textures/door.bmp", 254, 367);
    textures[18] = load_texture("textures/baluster.bmp", 100, 100);
    textures[19] = load_texture("textures/logo.bmp", 183, 275);
    textures[20] = load_texture("textures/cloud.bmp", 189, 115);
    textures[21] = load_texture("textures/sky.bmp", 387, 195);
    textures[22] = load_texture("textures/leaves2.bmp", 960, 720);
    textures[23] = load_texture("textures/wall.bmp", 260, 227);
    textures[24] = load_texture("textures/dambed2.bmp", 448, 448);
    textures[25] = load_texture("textures/pillar2.bmp", 1024, 1024);
    textures[26] = load_texture("textures/wall3.bmp", 900, 1234);
    textures[27] = load_texture("textures/grass.bmp", 712, 409);
    textures[28] = load_texture("textures/leaves3.bmp", 1600, 1600);
}

void initialise() {
    glClearColor(0.1, 0.6, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    draw_ground();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1000, 230, 2000);
    draw_building();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(800, 0, 5000);
    centre();
    glPopMatrix();

    glutSwapBuffers();
}

void spin_display() {
    if (jet_enable == 1)
        blob_x += jet_speed;

    if (blob_x > 400)
        blob_x = -30;
    glutPostRedisplay();
}

void timer(int n) {
    spin_display();
    glutTimerFunc(n, timer, n);
}

void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1 / zoom, 1 / zoom, -0.8 / zoom, 0.8 / zoom, 1, 100000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z, centre_x, centre_y, centre_z, up_x, up_y, up_z);
}

void special(int key, int x, int y) {
    float dxyz = sqrt((centre_z - eye_z) * (centre_z - eye_z) + (centre_x - eye_x) * (centre_x - eye_x) + (centre_y - eye_y) * (centre_y - eye_y));
    float d0xz = sqrt((centre_z - eye_z) * (centre_z - eye_z) + (centre_x - eye_x) * (centre_x - eye_x));

    switch (key) {
        case GLUT_KEY_F8: {
            eye_x = 4000;
            eye_y = 2500;
            eye_z = -1000;
            centre_x = 4000;
            centre_y = 2400;
            centre_z = 0;
            up_x = 0;
            up_y = 1;
            up_z = 0;
            angle_xz = M_PI;
        } break;
        case GLUT_KEY_F9: {
            eye_x = 4000;
            eye_y = 150;
            eye_z = 15000;
            centre_x = 4000;
            centre_y = 150;
            centre_z = 14000;
            up_x = 0;
            up_y = 1;
            up_z = 0;
            angle_xz = 0;
        } break;
        case GLUT_KEY_LEFT: {
            angle_xz += angle_step;
            centre_x = eye_x - d0xz * sinf(angle_xz);
            centre_z = eye_z - d0xz * cosf(angle_xz);
        } break;
        case GLUT_KEY_RIGHT: {
            angle_xz -= angle_step;
            centre_x = eye_x - d0xz * sinf(angle_xz);
            centre_z = eye_z - d0xz * cosf(angle_xz);
        } break;
        case GLUT_KEY_DOWN: {
            if (angle_y > -M_PI / 3) {
                angle_y -= angle_step;
                float d1xz = dxyz * cosf(angle_y);
                centre_y = eye_y + dxyz * sinf(angle_y);
                centre_x = eye_x - d1xz * sinf(angle_xz);
                centre_z = eye_z - d1xz * cosf(angle_xz);
            }
        } break;
        case GLUT_KEY_UP: {
            if (angle_y < M_PI / 3) {
                angle_y += angle_step;
                float d1xz = dxyz * cosf(angle_y);
                centre_y = eye_y + dxyz * sinf(angle_y);
                centre_x = eye_x - d1xz * sinf(angle_xz);
                centre_z = eye_z - d1xz * cosf(angle_xz);
            }
        } break;
        case GLUT_KEY_HOME: {
            if (jet_speed < 15)
                jet_speed += 0.1;
        } break;
        case GLUT_KEY_END: {
            if (jet_speed > 0.5)
                jet_speed -= 0.1;
        } break;
        default:
            break;
    }

    if (angle_xz > 2 * M_PI)
        angle_xz = 0;
    if (angle_xz < -2 * M_PI)
        angle_xz = 0;

    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z, centre_x, centre_y, centre_z, up_x, up_y, up_z);
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    float cos = (eye_z - centre_z) / sqrt((centre_z - eye_z) * (centre_z - eye_z) + (centre_x - eye_x) * (centre_x - eye_x));
    float sin = (centre_x - eye_x) / sqrt((centre_z - eye_z) * (centre_z - eye_z) + (centre_x - eye_x) * (centre_x - eye_x));

    switch (key) {
        case 'a': {
            eye_x -= eye_step * cos;
            eye_z -= eye_step * sin;
            centre_x -= eye_step * cos;
            centre_z -= eye_step * sin;
        } break;
        case 'd': {
            eye_x += eye_step * cos;
            eye_z += eye_step * sin;
            centre_x += eye_step * cos;
            centre_z += eye_step * sin;
        } break;
        case 'w': {
            eye_x += eye_step * sin;
            eye_z -= eye_step * cos;
            centre_x += eye_step * sin;
            centre_z -= eye_step * cos;
        } break;
        case 's': {
            eye_x -= eye_step * sin;
            eye_z += eye_step * cos;
            centre_x -= eye_step * sin;
            centre_z += eye_step * cos;
        } break;
        case 'r': {
            eye_y += eye_step;
            centre_y += eye_step;
        } break;
        case 'f': {
            eye_y -= eye_step;
            centre_y -= eye_step;
        } break;
        case '+': {
            if (eye_step < 100) {
                eye_step += 5;
                angle_step = M_PI / (120 - eye_step);
            }
        } break;
        case '-': {
            if (eye_step > 5) {
                eye_step -= 5;
                angle_step = M_PI / (220 - 2 * eye_step);
            }
        } break;

        default:
            break;
    }

    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z, centre_x, centre_y, centre_z, up_x, up_y, up_z);
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                jet_enable = 1 - jet_enable;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                if (zoom < 4) {
                    zoom *= 2;
                } else
                    zoom = 1;

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glFrustum(-1 / zoom, 1 / zoom, -0.8 / zoom, 0.8 / zoom, 1, 100000);
                glMatrixMode(GL_MODELVIEW);
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN) {
                zoom = 1;

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glFrustum(-1 / zoom, 1 / zoom, -0.8 / zoom, 0.8 / zoom, 1, 100000);
                glMatrixMode(GL_MODELVIEW);
            }
            break;
        default:
            break;
    }

    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z, centre_x, centre_y, centre_z, up_x, up_y, up_z);
    glutPostRedisplay();
}

void draw_ground() {
    glPushMatrix();
    up_to_down_square_face(8000, 16500, 80, 165, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 3960);
    outer_box(200, 10, 12340, 1, 0.05, 61.7, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(7800, 0, 3960);
    outer_box(200, 10, 12340, 1, 0.05, 61.7, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 16300);
    outer_box(8000, 10, 200, 40, 0.05, 1, 0, 0, 0, 0, 0, 0);
    glPopMatrix();
}

void draw_building() {
    glPushMatrix();
    draw_building_ground();
    glPopMatrix();

    glPushMatrix();
    draw_building_trees();
    glPopMatrix();

    glPushMatrix();
    draw_building_floor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2300, 0, -500);
    draw_hall_floor();
    glPopMatrix();

    glPushMatrix();
    draw_rooms();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2300, 0, -500);
    draw_back_porch();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2400, 0, 600);
    draw_vestibule();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3600, 0, 600);
    draw_stair_space();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-50, 1390, -50);
    draw_building_ceiling();
    glPopMatrix();
}

void draw_building_ground() {
    glPushMatrix();
    glTranslatef(-1000, -230, -2000);
    outer_box(8000, 150, 3800, 160, 2.4, 76, 1, 1, 1, 1, 1, 24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1000, -80, -2000);
    outer_box(8000, 10, 200, 40, 0.05, 1, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1000, -80, -1800);
    outer_box(200, 10, 3600, 1, 0.05, 18, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6800, -80, -1800);
    outer_box(200, 10, 3600, 1, 0.05, 18, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -80, 750);
    outer_box(850, 10, 1050, 4.25, 0.05, 5.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1550, -80, 750);
    outer_box(600, 10, 1050, 3, 0.05, 5.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4150, -80, 750);
    outer_box(600, 10, 1050, 3, 0.05, 5.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5450, -80, 750);
    outer_box(550, 10, 1050, 2.75, 0.05, 5.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    for (int i = 1; i < 5; i++) {
        glPushMatrix();
        glTranslatef(-1000, -110 - 30 * i, 1800);
        outer_box(8000, 30, 40 * i, 160, 0.6, 0.8 * i, 1, 1, 1, 1, 1, 24);
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_building_trees() {
    glPushMatrix();
    glTranslatef(425, -70, 1275);
    big_tree(35, 800, 4, 200, 300, 6, 5, 2, 28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1850, -70, 1275);
    small_tree(25, 12, 190, 150, 2, 5, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4450, -70, 1275);
    small_tree(25, 12, 190, 150, 2, 5, 2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5725, -70, 1275);
    big_tree(35, 800, 4, 200, 300, 6, 5, 2, 28);
    glPopMatrix();
}

void draw_building_floor() {
    glPushMatrix();
    glTranslatef(0, -230, 0);
    outer_box(6000, 230, 600, 120, 4.6, 12, 12, 12, 12, 12, 1, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2300, -20, -500);
    outer_box(1400, 20, 500, 28, 0.4, 10, 1, 1, 1, 1, 1, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2300, -20, 600);
    outer_box(1700, 20, 500, 34, 0.4, 10, 1, 1, 1, 1, 1, 20);
    glPopMatrix();

    for (int i = 1; i < 3; i++) {
        glPushMatrix();
        glTranslatef(2300 - 50 * i, -20 - 30 * i, -500 - 40 * i);
        outer_box(1400 + 100 * i, 30, 500 + 40 * i, 28 + 2 * i, 0.6, 10 + 0.8 * i, 1, 1, 1, 1, 1, 24);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2300 - 50 * i, -20 - 30 * i, 600);
        outer_box(1700 + 100 * i, 30, 500 + 40 * i, 34 + 2 * i, 0.6, 10 + 0.8 * i, 1, 1, 1, 1, 1, 24);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(900, -20 - 30 * i, 600);
        outer_box(600, 30, 40 * i, 12, 0.6, 0.8 * i, 1, 1, 1, 1, 1, 24);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(4800, -20 - 30 * i, 600);
        outer_box(600, 30, 40 * i, 12, 0.6, 0.8 * i, 1, 1, 1, 1, 1, 24);
        glPopMatrix();
    }
}

void draw_hall_floor() {
    glPushMatrix();
    outer_box(1400, 10, 1100, 28, 0.2, 22, 1, 1, 1, 1, 1, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 1100);
    outer_box(1700, 10, 500, 34, 0.2, 10, 1, 1, 1, 1, 1, 20);
    glPopMatrix();
}

void draw_rooms() {
    glPushMatrix();
    room("leftedge");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(600, 0, 0);
    room("righthiden");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1200, 0, 0);
    room("lefthiden");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1800, 0, 0);
    room("on");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3600, 0, 0);
    room("lefthiden");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4200, 0, 0);
    room("on");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(4800, 0, 0);
    room("allhiden");
    glPopMatrix();

    glPushMatrix();
    glTranslatef(600 * 9, 0, 0);
    room("rightedge");
    glPopMatrix();

    glPushMatrix();
    for (int i = 1; i < 4; i++) {
        glPushMatrix();
        glTranslatef(0, 350 * i, 0);
        room("leftedge");
        glPopMatrix();

        glPushMatrix();
        for (int j = 1; j < 4; j++) {
            glPushMatrix();
            glTranslatef(600 * j, 350 * i, 0);
            room("on");
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        for (int j = 4; j < 6; j++) {
            glPushMatrix();
            glTranslatef(600 * j, 350 * i, 0);
            room("allhiden");
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(600 * 6, 350 * i, 0);
        room("lefthiden");
        glPopMatrix();

        glPushMatrix();
        for (int j = 7; j < 9; j++) {
            glPushMatrix();
            glTranslatef(600 * j, 350 * i, 0);
            room("on");
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        glTranslatef(600 * 9, 350 * i, 0);
        room("rightedge");
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    for (int k = 0; k < 2; k++) {
        glPushMatrix();
        glTranslatef(2400, 0, 570 * k);
        small_pillar();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2970, 0, 570 * k);
        big_pillar();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(3570, 0, 570 * k);
        small_pillar();
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_back_porch() {
    glPushMatrix();
    glTranslatef(0, 340, 0);
    outer_box(1400, 20, 1100, 28, 0.4, 22, 12, 12, 12, 12, 26, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(670, 0, 470);
    back_porch_pillars();
    glPopMatrix();
}

void draw_vestibule() {
    glPushMatrix();
    glTranslated(0, 350, 0);
    draw_vestibule_panes();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(570, 0, 0);
    draw_vestibule_pillars();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-100, 340, 0);
    covered();
    glPopMatrix();
}
void draw_vestibule_panes() {
    glPushMatrix();
    outer_box(1200, 1050, 400, 24, 21, 8, 23, 23, 23, 23, 26, 23);
    glPopMatrix();

    glPushMatrix();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            glPushMatrix();
            glTranslatef(50 + 400 * i, 25 + 350 * j, 405);
            front_to_back_square_face(300, 300, 1, 1, 18);
            glPopMatrix();
        }
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(475, 300, 410);
    front_to_back_square_face(250, 450, 1, 1, 19);
    glPopMatrix();
}

void draw_vestibule_pillars() {
    glPushMatrix();
    big_pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-570, 0, 370);
    small_pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(600, 0, 370);
    small_pillar();
    glPopMatrix();
}

void draw_stair_space() {
    glPushMatrix();
    outer_box(300, 1400, 400, 6, 28, 8, 23, 23, 23, 23, 23, 23);
    glPopMatrix();

    glPushMatrix();
    for (int j = 0; j < 4; j++) {
        glPushMatrix();
        glTranslatef(303, 25 + 350 * j, 50);
        right_to_left_square_face(300, 300, 1, 1, 18);
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_building_ceiling() {
    glPushMatrix();
    outer_box(6100, 20, 700, 122, 0.4, 14, 12, 12, 12, 12, 26, 23);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2400, 0, 650);
    outer_box(1600, 20, 450, 32, 0.4, 9, 12, 12, 12, 12, 26, 23);
    glPopMatrix();
}

void up_to_down_square_face(float x, float z, float x_repeat, float z_repeat, int tex) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, z_repeat);
        glVertex3f(0, 0, 0);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, z);
        glTexCoord2f(x_repeat, 0);
        glVertex3f(x, 0, z);
        glTexCoord2f(x_repeat, z_repeat);
        glVertex3f(x, 0, 0);
    }
    glEnd();
    glPopMatrix();
}

void down_to_up_square_face(float x, float z, float x_repeat, float z_repeat, int tex) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);

        glVertex3f(0, 0, 0);
        glTexCoord2f(0, z_repeat);
        glVertex3f(0, 0, z);
        glTexCoord2f(x_repeat, z_repeat);

        glVertex3f(x, 0, z);
        glTexCoord2f(x_repeat, 0);
        glVertex3f(x, 0, 0);
    }
    glEnd();
    glPopMatrix();
}

void front_to_back_square_face(float x, float y, float x_repeat, float y_repeat, int tex) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
        glTexCoord2f(x_repeat, 0);
        glVertex3f(x, 0, 0);
        glTexCoord2f(x_repeat, y_repeat);
        glVertex3f(x, y, 0);
        glTexCoord2f(0, y_repeat);
        glVertex3f(0, y, 0);
    }
    glEnd();
    glPopMatrix();
}

void back_to_front_square_face(float x, float y, float x_repeat, float y_repeat, int tex) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(x_repeat, 0);
        glVertex3f(0, 0, 0);
        glTexCoord2f(0, 0);
        glVertex3f(x, 0, 0);
        glTexCoord2f(0, y_repeat);
        glVertex3f(x, y, 0);
        glTexCoord2f(x_repeat, y_repeat);
        glVertex3f(0, y, 0);
    }
    glEnd();
    glPopMatrix();
}

void right_to_left_square_face(float y, float z, float y_repeat, float z_repeat, int tex) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(z_repeat, 0);
        glVertex3f(0, 0, 0);
        glTexCoord2f(z_repeat, y_repeat);
        glVertex3f(0, y, 0);
        glTexCoord2f(0, y_repeat);
        glVertex3f(0, y, z);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, z);
    }
    glEnd();
    glPopMatrix();
}

void left_to_right_square_face(float y, float z, float y_repeat, float z_repeat, int tex) {
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
        glTexCoord2f(0, y_repeat);
        glVertex3f(0, y, 0);
        glTexCoord2f(z_repeat, y_repeat);
        glVertex3f(0, y, z);
        glTexCoord2f(z_repeat, 0);
        glVertex3f(0, 0, z);
    }
    glEnd();
    glPopMatrix();
}

void inner_box(float x, float y, float z, float x_repeat, float y_repeat, float z_repeat, int left, int right, int back, int front, int under, int upper) {
    glPushMatrix();
    right_to_left_square_face(y, z, y_repeat, z_repeat, left);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, 0, 0);
    left_to_right_square_face(y, z, y_repeat, z_repeat, right);
    glPopMatrix();

    glPushMatrix();
    front_to_back_square_face(x, y, x_repeat, y_repeat, back);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, z);
    back_to_front_square_face(x, y, x_repeat, y_repeat, front);
    glPopMatrix();

    glPushMatrix();
    up_to_down_square_face(x, z, x_repeat, z_repeat, under);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, y, 0);
    down_to_up_square_face(x, z, x_repeat, z_repeat, upper);
    glPopMatrix();
}

void outer_box(float x, float y, float z, float x_repeat, float y_repeat, float z_repeat, int left, int right, int back, int front, int under, int upper) {
    glPushMatrix();
    left_to_right_square_face(y, z, y_repeat, z_repeat, left);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, 0, 0);
    right_to_left_square_face(y, z, y_repeat, z_repeat, right);
    glPopMatrix();

    glPushMatrix();
    back_to_front_square_face(x, y, x_repeat, y_repeat, back);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, z);
    front_to_back_square_face(x, y, x_repeat, y_repeat, front);
    glPopMatrix();

    glPushMatrix();
    down_to_up_square_face(x, z, x_repeat, z_repeat, under);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, y, 0);
    up_to_down_square_face(x, z, x_repeat, z_repeat, upper);
    glPopMatrix();
}

void vertical_cylinder(float base_radius, float top_radius, float height, float xz_repeat, float y_repeat, int xz_split, int y_split, int tex) {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(xz_repeat, y_repeat, 1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    GLUquadricObj* obj;
    obj = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    gluQuadricTexture(obj, GL_TRUE);
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);
    glRotatef(-90, 1, 0, 0);
    glColor3f(0, 0, 1);
    gluCylinder(obj, base_radius, top_radius, height, xz_split, y_split);
    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void vertical_sphere(float radius, float xz_repeat, float y_repeat, int xz_split, int y_split, int tex) {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(xz_repeat, y_repeat, 1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    GLUquadricObj* obj;
    obj = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    gluQuadricTexture(obj, GL_TRUE);
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);
    glRotatef(-90, 1, 0, 0);
    gluSphere(obj, radius, xz_split, y_split);
    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void horizonal_disk(float inner_radius, float outer_radius, float x_repeat, float z_repeat, int xz_split, int y_split, int tex) {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(x_repeat, z_repeat, 1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    GLUquadricObj* obj;
    obj = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    gluQuadricTexture(obj, GL_TRUE);
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);
    glRotatef(-90, 1, 0, 0);
    gluDisk(obj, inner_radius, outer_radius, xz_split, y_split);
    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void partial_horizonal_disk(float inner_radius, float outer_radius, float start_angle, float sweep_angle, float x_repeat, float z_repeat, int xz_split, int y_split, int tex) {
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(x_repeat, z_repeat, 1);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    GLUquadricObj* obj;
    obj = gluNewQuadric();
    glBindTexture(GL_TEXTURE_2D, textures[tex]);
    gluQuadricTexture(obj, GL_TRUE);
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GLU_SMOOTH);
    glRotatef(-90, 1, 0, 0);
    gluPartialDisk(obj, inner_radius, outer_radius, xz_split, y_split, start_angle, sweep_angle);
    glPopMatrix();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1, 1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void room_frame() {
    glPushMatrix();
    outer_box(600, 350, 400, 12, 7, 8, 23, 23, 23, 25, 26, 23);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10, 10, 10);
    inner_box(580, 330, 380, 11.6, 6.6, 7.6, 23, 23, 23, 23, 20, 26);
    glPopMatrix();
}

void eaves() {
    glPushMatrix();
    outer_box(600, 10, 200, 12, 0.2, 4, 12, 12, 12, 12, 26, 23);
    glPopMatrix();
}

void corridor() {
    glPushMatrix();
    outer_box(600, 10, 200, 12, 0.2, 4, 12, 12, 12, 12, 26, 20);
    glPopMatrix();
}

void small_pillar() {
    glPushMatrix();
    outer_box(30, 350, 30, 0.6, 7, 0.6, 12, 12, 12, 12, 12, 12);
    glPopMatrix();
}

void big_pillar() {
    glPushMatrix();
    outer_box(60, 350, 30, 1.2, 7, 0.6, 12, 12, 12, 12, 12, 12);
    glPopMatrix();
}

void room_pillars() {
    glPushMatrix();
    small_pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(285, 0, 0);
    small_pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(570, 0, 0);
    small_pillar();
    glPopMatrix();
}

void left_front_handrail() {
    glPushMatrix();
    outer_box(290, 10, 10, 11.6, 0.2, 0.2, 6, 6, 6, 6, 6, 6);
    glPopMatrix();

    glPushMatrix();
    for (int i = 0; i < 16; i++) {
        glPushMatrix();
        glTranslatef(34 + 15 * i, -125, 4);
        outer_box(2, 130, 2, 0.04, 2.6, 0.04, 6, 6, 6, 6, 6, 6);
        glPopMatrix();
    }
    glPopMatrix();
}

void right_front_handrail() {
    glPushMatrix();
    glTranslatef(290, 0, 0);
    outer_box(290, 10, 10, 11.6, 0.2, 0.2, 6, 6, 6, 6, 6, 6);
    glPopMatrix();

    glPushMatrix();
    for (int i = 19; i < 35; i++) {
        glPushMatrix();
        glTranslatef(34 + 15 * i, -125, 4);
        outer_box(2, 130, 2, 0.04, 2.6, 0.04, 6, 6, 6, 6, 6, 6);
        glPopMatrix();
    }
    glPopMatrix();
}

void edge_handrail() {
    glPushMatrix();
    outer_box(10, 10, 195, 0.2, 0.2, 3.9, 6, 6, 6, 6, 6, 6);
    glPopMatrix();

    for (int i = 0; i < 9; i++) {
        glPushMatrix();
        glTranslatef(4, -125, 21 + 17 * i);
        outer_box(2, 130, 2, 0.04, 2.6, 0.04, 6, 6, 6, 6, 6, 6);
        glPopMatrix();
    }
}

void window() {
    glPushMatrix();
    front_to_back_square_face(460, 180, 2.5, 1, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 14);
    front_to_back_square_face(460, 180, 2.6, 1, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 392);
    front_to_back_square_face(360, 180, 2, 1, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 406);
    front_to_back_square_face(360, 180, 2, 1, 16);
    glPopMatrix();
}

void door() {
    glPushMatrix();
    front_to_back_square_face(120, 280, 1, 1, 17);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -14);
    front_to_back_square_face(120, 280, 1, 1, 17);
    glPopMatrix();
}

void room(std::string type) {
    glPushMatrix();
    room_frame();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 340, 400);
    eaves();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 400);
    corridor();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 570);
    room_pillars();
    glPopMatrix();

    if (type != "allhiden") {
        if (type != "lefthiden") {
            glPushMatrix();
            glTranslatef(10, 130, 580);
            left_front_handrail();
            glPopMatrix();
        }

        if (type != "righthiden") {
            glPushMatrix();
            glTranslatef(10, 130, 580);
            right_front_handrail();
            glPopMatrix();
        }
    }

    if (type == "leftedge") {
        glPushMatrix();
        glTranslatef(10, 130, 395);
        edge_handrail();
        glPopMatrix();
    }

    if (type == "rightedge") {
        glPushMatrix();
        glTranslatef(580, 130, 395);
        edge_handrail();
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(30, 110, -3);
    window();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(450, 10, 403);
    door();
    glPopMatrix();
}

void covered() {
    glPushMatrix();
    outer_box(1700, 20, 500, 34, 0.4, 10, 12, 12, 12, 12, 26, 20);
    glPopMatrix();
}

void back_porch_pillars() {
    glPushMatrix();
    big_pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-570, 0, -370);
    small_pillar();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(600, 0, -370);
    small_pillar();
    glPopMatrix();
}

void centre_ground() {
    glPushMatrix();
    outer_box(6400, 10, 10500, 128, 0.2, 210, 1, 1, 1, 1, 1, 24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300, 10, 3300);
    outer_box(2400, 90, 900, 48, 1.8, 18, 1, 1, 1, 1, 1, 24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3700, 10, 3300);
    outer_box(2400, 90, 900, 48, 1.8, 18, 1, 1, 1, 1, 1, 24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300, 10, 6720);
    outer_box(2400, 90, 900, 48, 1.8, 18, 1, 1, 1, 1, 1, 24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3700, 10, 6720);
    outer_box(2400, 90, 900, 48, 1.8, 18, 1, 1, 1, 1, 1, 24);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300, 10, 4200);
    outer_box(5800, 90, 2520, 116, 1.8, 50.4, 1, 1, 1, 1, 1, 24);
    glPopMatrix();

    glPushMatrix();
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(2700, 10 + 30 * i, 4120 + 30 * i);
        outer_box(1000, 30, 80 - 40 * i, 20, 0.6, 1.6 - 0.8 * i, 1, 1, 1, 1, 1, 7);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2700, 10 + 30 * i, 6720);
        outer_box(1000, 30, 80 - 40 * i, 20, 0.6, 1.6 - 0.8 * i, 1, 1, 1, 1, 1, 7);
        glPopMatrix();
    }
    glPopMatrix();
}

void centre_grass() {
    glPushMatrix();
    glTranslatef(300, 10, 660);
    outer_box(2400, 10, 2400, 6, 0.025, 6, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3700, 10, 660);
    outer_box(2400, 10, 2400, 6, 0.025, 6, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300, 10, 7860);
    outer_box(2400, 10, 2400, 6, 0.025, 6, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3700, 10, 7860);
    outer_box(2400, 10, 2400, 6, 0.025, 6, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2700, 110, 5460);
    partial_horizonal_disk(1260, 2160, 180, 180, 10.8, 10.8, 50, 1, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3700, 110, 5460);
    partial_horizonal_disk(1260, 2160, 0, 180, 10.8, 10.8, 50, 1, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300, 100, 3300);
    outer_box(2400, 10, 900, 6, 0.025, 2.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3700, 100, 3300);
    outer_box(2400, 10, 900, 6, 0.025, 2.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300, 100, 4200);
    outer_box(800, 10, 2520, 2, 0.025, 6.3, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(5300, 100, 4200);
    outer_box(800, 10, 2520, 2, 0.025, 6.3, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(300, 100, 6720);
    outer_box(2400, 10, 900, 6, 0.025, 2.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3700, 100, 6720);
    outer_box(2400, 10, 900, 6, 0.025, 2.25, 0, 0, 0, 0, 0, 0);
    glPopMatrix();
}

void stone() {
    glPushMatrix();
    outer_box(480, 10, 250, 1.2, 0.025, 0.625, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 390);
    outer_box(480, 10, 250, 1.2, 0.025, 0.625, 0, 0, 0, 0, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 250);
    outer_box(480, 200, 140, 1, 1, 1, 11, 11, 11, 10, 11, 11);
    glPopMatrix();
}

void flags() {
    glPushMatrix();
    for (int i = 0; i < 15; i++) {
        glPushMatrix();
        glTranslatef(2660 - 130 * i, 20, 700);
        flag();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(3740 + 130 * i, 20, 700);
        flag();
        glPopMatrix();
    }
    glPopMatrix();
}

void centre_small_trees() {
    glPushMatrix();
    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(400 + 900 * i, 20, 2960);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(6000 - 900 * i, 20, 2960);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(400 + 900 * i, 110, 3400);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(6000 - 900 * i, 110, 3400);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(400 + 900 * i, 110, 7520);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(6000 - 900 * i, 110, 7520);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(400 + 900 * i, 20, 7960);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glTranslatef(6000 - 900 * i, 20, 7960);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(400 + 5600 * i, 110, 4300);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(400 + 5600 * i, 110, 6620);
        small_tree(10, 5, 120, 100, 2, 5, 2);
        glPopMatrix();
    }
    glPopMatrix();
}

void centre_big_trees() {
    glPushMatrix();
    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(400 + 5600 * i, 20, 760);
        big_tree(30, 500, 2.5, 200, 200, 5, 5, 2, 22);
        glPopMatrix();
    }

    for (int i = 0; i < 5; i++) {
        if (i != 2) {
            for (int j = 0; j < 3; j++) {
                glPushMatrix();
                glTranslatef(400 + 1400 * i, 20, 10160 - 900 * j);
                big_tree(20, 1000, 4, 250, 150, 7, 5, 2, 2);
                glPopMatrix();
            }
        }
    }

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(400 + 5600 * i, 110, 5460);
        big_tree(30, 500, 2.5, 200, 200, 5, 5, 2, 22);
        glPopMatrix();
    }

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(1300, 110, 4560 + 1800 * i);
        big_tree(30, 500, 2.5, 200, 200, 5, 5, 2, 22);
        glPopMatrix();
    }

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(5100, 110, 4560 + 1800 * i);
        big_tree(30, 500, 2.5, 200, 200, 5, 5, 2, 22);
        glPopMatrix();
    }
    glPopMatrix();
}

void centre_trees() {
    glPushMatrix();
    centre_small_trees();
    centre_big_trees();
    glPopMatrix();
}

void dam() {
    glPushMatrix();
    vertical_cylinder(750, 750, 100, 30 * M_PI, 1, 50, 1, 1);
    glPopMatrix();

    glPushMatrix();
    vertical_cylinder(700, 700, 100, 28 * M_PI, 1, 50, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 100, 0);
    horizonal_disk(700, 750, 15, 15, 50, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 50, 0);
    horizonal_disk(0, 700, 15, 15, 50, 1, 14);
    glPopMatrix();
}

void water_spout() {
    glPushMatrix();
    for (int i = 0; i < 360; i += 15) {
        glPushMatrix();
        glRotatef(i, 0, 1, 0);
        {
            glPushMatrix();
            glTranslatef(-100, 0, 0);
            glPushMatrix();
            glRotatef(-17, 0, 0, 1);
            vertical_cylinder(3, 3, 20, 1, 1, 20, 1, 1);
            glPopMatrix();
            glPopMatrix();
        }

        {
            glPushMatrix();
            glTranslatef(-683, 0, 0);
            glPushMatrix();
            glRotatef(-30, 0, 0, 1);
            vertical_cylinder(3, 3, 20, 1, 1, 20, 1, 1);
            glPopMatrix();
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void jet() {
    glPushMatrix();
    for (int i = 0; i < 360; i += 15) {
        for (int j = 1; j < 500; j++) {
            glPushMatrix();
            glRotatef(i, 0, 1, 0);
            glPushMatrix();
            float x = blob_x - 0.6 * j - i / 15;

            if (x > -17.32 && x < 17.32) {
                float jetfunc = 300 - x * x;
                glTranslatef(73.5 + 10 * x, jetfunc, 0);
                vertical_sphere(2.5, 1, 1, 6, 4, 20);
            }
            glPopMatrix();
            glPopMatrix();

            glPushMatrix();
            glRotatef(i, 0, 1, 0);
            glPushMatrix();
            x = blob_x - 50 - 0.4 * j - i / 15;

            if (x > -12.25 && x < 12.25) {
                float jetfunc = 150 - x * x;
                glTranslatef(-500 + 15 * x, jetfunc, 0);
                vertical_sphere(2.5, 1, 1, 5, 3, 20);
            }
            glPopMatrix();
            glPopMatrix();
        }
    }
    glPopMatrix();
}

void jets() {
    glPushMatrix();
    jet();
    glPopMatrix();
}

void centre() {
    glPushMatrix();
    centre_ground();
    glPopMatrix();

    glPushMatrix();
    centre_grass();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2960, 10, 8740);
    stone();
    glPopMatrix();

    glPushMatrix();
    flags();
    glPopMatrix();

    glPushMatrix();
    centre_trees();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3200, 100, 5460);
    dam();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3200, 149, 5460);
    water_spout();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3200, 149, 5460);
    jets();
    glPopMatrix();
}

void flag() {
    glPushMatrix();
    vertical_cylinder(10, 5, 50, 1, 1, 30, 1, 7);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 50, 0);
    vertical_cylinder(5, 3, 50, 1, 1, 30, 1, 7);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 100, 0);
    vertical_cylinder(3, 3, 500, 1, 1, 30, 1, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 450, 0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, textures[5]);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0);
        glVertex3f(100, -25, 130);
        glTexCoord2f(1, 1);
        glVertex3f(100, 75, 130);
        glTexCoord2f(0, 1);
        glVertex3f(0, 100, 0);
    }
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void small_tree(float base_radius, float top_radius, float height, float radius, float rind_repeat, float xz_leaves_repeat, float y_leaves_repeat) {
    glPushMatrix();
    vertical_cylinder(base_radius, top_radius, height, 1, rind_repeat, 40, 1, 3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, height, 0);
    vertical_sphere(radius, xz_leaves_repeat, y_leaves_repeat, 50, 10, 28);
    glPopMatrix();
}

void big_tree(float base_radius, float rind_height, float rind_repeat, float root_height, float radius, int canopy, float xz_leaves_repeat, float y_leaves_repeat, int tex) {
    glPushMatrix();
    vertical_cylinder(base_radius, 0, rind_height + root_height, 1, rind_repeat, 40, 1, 3);
    glPopMatrix();

    glPushMatrix();
    for (int i = 0; i < canopy; i++) {
        glPushMatrix();
        glTranslatef(0, root_height + rind_height * i / canopy, 0);
        vertical_cylinder(radius * (canopy - i) / canopy, 0, rind_height / (canopy - 1), xz_leaves_repeat, y_leaves_repeat, 50, 1, tex);
        vertical_cylinder(radius * (canopy - i) / canopy, 0, rind_height / (2 * canopy), xz_leaves_repeat, y_leaves_repeat, 50, 1, 22);
        glPopMatrix();
    }
    glPopMatrix();
}
