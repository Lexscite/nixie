#ifndef VOODOO_MESH_CONVERTER_H_
#define VOODOO_MESH_CONVERTER_H_

#include <voodoo/vector.h>

namespace voodoo
{
	struct Face
	{
		Vector3<int> v;
		Vector3<int> t;
		Vector3<int> n;
	};

	class ModelConverter final
	{
	public:
		static ModelConverter* Get();

		bool Init();
		bool Run();

	private:
		ModelConverter() = default;

		bool CheckFile(char* filename);
		bool ProcessFile(char* filename);

		bool GetModelMetrics(char* filename);
		bool Read(char* filename);
		bool Write(char* filename);

	private:
		static ModelConverter* singleton_;

		int v_count_, t_count_, n_count_, f_count_;

		std::vector<Vector3<float>> v_coords_;
		std::vector<Vector3<float>> t_coords_;
		std::vector<Vector3<float>> n_coords_;
		std::vector<Face> faces_;
	};
}

#endif