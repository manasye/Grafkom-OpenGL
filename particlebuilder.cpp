#include <GL/glew.h>
#include <stdlib.h>
#include "particlebuilder.hpp"
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

/**
 * Reference : http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing
 **/
GLuint loadTextureFromFile(const char* filename)
{
	GLuint texture;
	glGenTextures(1, &texture);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

	if (data){ //if data is exist and success loaded
		glBindTexture(GL_TEXTURE_2D, texture);
		// Parameters used specifying how the texture should be generated
		// Also tells how the mipmaps should be generated as well
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// The actual conversion from image to textures
        if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
        printf("[WARN] Failed to load %s ...\n",filename);
	}

	stbi_image_free(data);
	return texture;
}

ParticleBuilder::ParticleBuilder()
{
	//do nothing
}

ParticleBuilder::ParticleBuilder(unsigned int amount, const char*textureFile)
{
	this->texture = loadTextureFromFile(textureFile);
	this->last_used_particle = 0;
    this->amount = amount;

    // The VBO containing the 4 vertices of the particles.
    static const GLfloat g_vertex_buffer_data[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
    };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // The VBO containing the positions and sizes of the particles
    GLuint particles_position_buffer;
    glGenBuffers(1, &particles_position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);

    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, amount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    // The VBO containing the colors of the particles
    GLuint particles_color_buffer;
    glGenBuffers(1, &particles_color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);

    // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    glBufferData(GL_ARRAY_BUFFER, amount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

    //1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

    // 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// These functions are specific to glDrawArrays*Instanced*.
	// The first parameter is the attribute buffer we're talking about.
	// The second parameter is the "rate at which generic vertex attributes advance when rendering multiple instances"
	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribDivisor.xml
	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad -> 1

	glBindVertexArray(0);

	//Init particle with amount size
	for(auto i = 0; i < amount; i++)
	{
		particles.push_back(Particle());
	}

	//allocate memory in vector
	g_particle_position_size_data.resize(4 * amount); // x,y,z,size
	g_particle_color_data.resize(4 * amount); //r,g,b,a
}

void ParticleBuilder::draw()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(GLfloat) * 4, &g_particle_position_size_data[0]);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(GLubyte) * 4, &g_particle_color_data[0]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
	glBindVertexArray(0);
}

void ParticleBuilder::sortParticles()
{
	std::sort(particles.begin(), particles.end());
}

unsigned int ParticleBuilder::findUnusedParticle()
{
	for(unsigned int i = last_used_particle; i < amount; i++)
	{
		if (particles[i].life < 0.0f) {
			last_used_particle = i;
			return last_used_particle;
		}
	}

	for(unsigned int i = 0; i < last_used_particle; i++)
	{
		if (particles[i].life < 0.0f) {
			last_used_particle = i;
			return last_used_particle;
		}
	}
	
	last_used_particle = 0;
	return last_used_particle;
}

void ParticleBuilder::update(float delta, glm::vec3 camera_pos)
{
	count = 0;
	for(unsigned int i = 0; i < amount; i++)
	{
		Particle &p = particles[i];
		if (p.life > 0.0f){
			p.life -= delta;
			if (p.life > 0.0f){
				p.speed += glm::vec3(0.0f,-9.81f, 0.0f) * (float)delta * 0.5f;
				p.pos += p.speed * (float)delta;
            	p.cameradistance = glm::length2(p.pos - camera_pos);

				// Fill the GPU buffer
				g_particle_position_size_data[4*count+0] = p.pos.x;
				g_particle_position_size_data[4*count+1] = p.pos.y;
				g_particle_position_size_data[4*count+2] = p.pos.z;

				g_particle_position_size_data[4*count+3] = p.size;

				g_particle_color_data[4*count+0] = p.color.r;
				g_particle_color_data[4*count+1] = p.color.g;
				g_particle_color_data[4*count+2] = p.color.b;
				g_particle_color_data[4*count+3] = p.color.a;
			} else {
				p.cameradistance = -1.0f;
			}

			count++;
		}
	}
	
	sortParticles();
}