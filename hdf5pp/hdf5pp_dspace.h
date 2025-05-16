#pragma once

#include "hdf5pp_handle.h"
#include "hdf5pp_proplist.h"

namespace HDF5 {

	class HDF5PP_API Dataspace : public Handle
	{
	public:
		Dataspace(const Dataspace& rhs);

		enum Type {
			Scalar = H5S_SCALAR,
			Simple = H5S_SIMPLE,
			Null = H5S_NULL
		};
		// Create a Dataspace of a specified type
		Dataspace(Type type = Type::Scalar);

		// Create a simple Dataspace
		Dataspace(const std::vector<hsize_t>& currentDims, const std::vector<hsize_t>& maxDims = std::vector<hsize_t>());

		// Decode a binary object description of a Dataspace
		Dataspace(unsigned char* buf);

		virtual ~Dataspace();
		Dataspace& operator=(const Dataspace& rhs);

		bool Attach(hid_t hid) override;

		// Creates an exact copy of the Dataspace
		Dataspace Copy();

		// Encode a data space object description into a binary buffer
		bool Encode(std::vector<unsigned char>& buf, const PropertyList& fapl = PropertyList());

		// Copies the extent of a Dataspace
		bool CopyExtent(const Dataspace& src_dspace);

		// Determines whether two Dataspace are equal
		bool ExtentEqual(const Dataspace& other_dspace);

		// Retrieves a regular hyperslab selection
		bool GetRegularHyperslab(std::vector<hsize_t>& start, std::vector<hsize_t>& count, std::vector<hsize_t>& stride, std::vector<hsize_t>& block);

		// Gets the bounding box containing the current selection
		bool GetSelectBounds(std::vector<hsize_t>& start, std::vector<hsize_t>& end);

		// Gets the number of element points in the current selection
		hssize_t GetSelectedElementsPointsCount();

		// Gets the list of element points currently selected
		bool GetSelectedElementsPoints(hsize_t startpoint, hsize_t numpoints, std::vector<hsize_t>& buf);

		// Gets the list of hyperslab blocks currently selected
		bool GetSelectedHyperslabBlocks(hsize_t startblock, hsize_t numblocs, std::vector<hsize_t>& buf);

		// Get number of hyperslab blocks
		hssize_t GetHyperslabBlocksCount();

		// Determines the number of elements in a Dataspace selection
		hssize_t GetSelectedElementsCount();

		enum class SelectionType {
			None = H5S_SEL_NONE,			// no selection is defined
			Points = H5S_SEL_POINTS,		// selection is a sequence of points
			Hyperslabs = H5S_SEL_HYPERSLABS,// selection is a hyperslab or compound hyperslabs
			All = H5S_SEL_ALL				// the entire Dataset is selected
		};
		// Determines the type of the Dataspace selection
		bool GetSelectionType(SelectionType& t);

		// Retrieves Dataspace dimension size and maximum size
		int GetSimpleExtentDims(hsize_t* dims, hsize_t* maxdims = nullptr);

		// Determines the dimensionality of a Dataspace
		int GetSimpleExtentDimsCount();

		// Determines the number of elements in a Dataspace
		hssize_t GetSimpleExtentElementsCount();

		// Determines the current class of a Dataspace
		bool GetSimpleExtentType(Type& t);

		// Determines whether a hyperslab selection is regular
		bool IsRegularHyperslab(bool* failed = nullptr);

		// Determines whether the Dataspace is a simple Dataspace
		bool IsSimple(bool* failed = nullptr);

		// Sets the offset of a simple Dataspace
		bool SetOffsetSimple(const hssize_t* offset);

		// Selects the entire Dataspace
		bool SelectAll();

		enum class SelectionOperation {
			Set = H5S_SELECT_SET,	// Replaces the existing selection with the parameters from this call. Overlapping blocks are not supported with this operator.
			Or = H5S_SELECT_OR,		// Adds the new selection to the existing selection. (Binary OR)
			And = H5S_SELECT_AND,	// Retains only the overlapping portions of the new selection and the existing selection. (Binary AND)
			Xor = H5S_SELECT_XOR,	// Retains only the elements that are members of the new selection or the existing selection, excluding elements that are members of both selections. (Binary exclusive-OR, XOR)
			NotB = H5S_SELECT_NOTB, // Retains only elements of the existing selection that are not in the new selection.
			NotA = H5S_SELECT_NOTA	// Retains only elements of the new selection that are not in the existing selection.
		};
		// Selects array elements to be included in the selection for the Dataspace
		bool SelectElements(SelectionOperation op, const std::vector<hsize_t>& coords);
		bool SelectElements(SelectionOperation op, size_t num_elements, const hsize_t* coords);

		// Selects a hyperslab region to add to the current selected region
		bool SelectHyperslab(SelectionOperation op, const hsize_t* start, const hsize_t* stride, const hsize_t* count, const hsize_t* block);

		// Resets the selection region to include no elements
		bool SelectNone();

		// Verifies that the selection is within the extent of the Dataspace
		bool IsSelectionValid(bool* failed = nullptr);

		// Removes the extent from the Dataspace
		bool SetExtentNone();

		// Sets or resets the size of the Dataspace
		bool SetExtentSimple(int rank, const hsize_t* dims, const hsize_t* max_dims = nullptr);
		bool SetExtentSimple(const std::vector<hsize_t>& currentDims, const std::vector<hsize_t>& maxDims = std::vector<hsize_t>());
	protected:
		explicit Dataspace(hid_t hid);
		friend class Attribute;
		friend class Dataset;
		friend class Location;
	};

}