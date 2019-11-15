#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->frame_mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face_mesh.clear();
	this->frame_mesh.clear();

	auto len = 300;
	auto width = 20;
	for (auto z = -len * 0.5 + width * 0.5; z <= len * 0.5 - width * 0.5; z += width * 2) {

		auto location = glm::vec3(0, 0, z);
		for (auto param = 0; param < 100; param++) {

			auto index = this->face_mesh.getVertices().size();
			vector<glm::vec3> vertices = {
				glm::vec3(this->make_point(len, param), width * -0.5),
				glm::vec3(this->make_point(len, param), width * 0.5),
				glm::vec3(this->make_point(len, param + 1), width * 0.5),
				glm::vec3(this->make_point(len, param + 1), width * -0.5)
			};

			for (auto& vertex : vertices) {

				auto noise_location = (location + vertex) * 0.00015;
				auto angle = glm::vec3(
					ofMap(ofNoise(noise_location.x, ofGetFrameNum() * 0.0015), 0, 1, -PI, PI),
					ofMap(ofGetFrameNum() % 360, 0, 360, -PI, PI),
					ofMap(ofNoise(noise_location.z, ofGetFrameNum() * 0.0025), 0, 1, -PI * 2, PI * 2)
				);

				auto rotation_x = glm::rotate(glm::mat4(), angle.x, glm::vec3(1, 0, 0));
				auto rotation_y = glm::rotate(glm::mat4(), angle.y, glm::vec3(0, 1, 0));
				auto rotation_z = glm::rotate(glm::mat4(), angle.z, glm::vec3(0, 0, 1));

				vertex = glm::vec4(location + vertex, 0) * rotation_z * rotation_y;

				this->face_mesh.addColor(ofColor(39));
				this->frame_mesh.addColor(ofColor(0, 0, 239));
			}

			this->face_mesh.addVertices(vertices);
			this->frame_mesh.addVertices(vertices);

			this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 1); this->face_mesh.addIndex(index + 2);
			this->face_mesh.addIndex(index + 0); this->face_mesh.addIndex(index + 2); this->face_mesh.addIndex(index + 3);

			this->frame_mesh.addIndex(index + 0); this->frame_mesh.addIndex(index + 3);
			this->frame_mesh.addIndex(index + 1); this->frame_mesh.addIndex(index + 2);

			if (param % 25 == 0) {

				this->frame_mesh.addIndex(index + 0); this->frame_mesh.addIndex(index + 1);
			}
		}
	}


}

//--------------------------------------------------------------
void ofApp::draw() {
	
	this->cam.begin();

	this->face_mesh.drawFaces();
	this->frame_mesh.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}