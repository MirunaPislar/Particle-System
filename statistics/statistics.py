import os
import matplotlib.pyplot as plt


def load_file(filename):
    file = open(filename, 'r')
    text = file.read()
    file.close()
    return text


def plot_simple(xp, yp, plot_name):
    plt.figure()
    plt.plot(xp, yp)
    plt.title("Gravity vs Frame Rate (when rendering as texture)")
    plt.xlabel('Gravity')
    plt.ylabel('Frame Rate')
    plt.savefig(plot_name)
    plt.show()


def plot_comparing(xp, yp, xt, yt, plot_name):
    plt.figure()
    plt.plot(xp, yp, 'k-', label='Data rendered as point')
    plt.plot(xt, yt, 'r--', label='Data rendered as texturess')
    plt.title('Particle Numbers vs Frame Rates (comparing points with textures)')
    plt.xlabel('Number of particles')
    plt.ylabel('Frame Rate')
    plt.legend(loc='center right')
    plt.savefig(plot_name)
    plt.show()


def plot_point_number_graphs():
    path = os.getcwd()[:os.getcwd().rfind('/')] + "/statistics_graphics/particles_no/"

    filename = path + "render_as_point.txt"
    data_rendered_as_point = load_file(filename).split("\n")

    point_particles = []
    point_fps = []

    for d in data_rendered_as_point:
        d = d.split()
        if float(d[0]) < 348501:
            point_particles.append(float(d[0]))
            point_fps.append(float(d[1]))

    filename = path + "render_as_texture.txt"
    data_rendered_as_texture = load_file(filename).split("\n")

    tex_particles = []
    tex_fps = []

    for d in data_rendered_as_texture:
        d = d.split()
        tex_particles.append(float(d[0]))
        tex_fps.append(float(d[1]))

    plot_comparing(point_particles, point_fps, tex_particles, tex_fps, path + "plot_point_vs_texture.png")


def plot_gravity():
    path = os.getcwd()[:os.getcwd().rfind('/')] + "/statistics_graphics/gravity/"
    filename = path + "render_as_texture.txt"
    data = load_file(filename).split("\n")

    gravity = []
    fps = []

    for d in data:
        d = d.split()
        gravity.append(float(d[0]))
        fps.append(float(d[1]))

    plot_simple(gravity, fps, path + "plot_gravity_render_as_texture.png")


def plot_results():
    plot_point_number_graphs()
    plot_gravity()


if __name__ == "__main__":
    plot_results()