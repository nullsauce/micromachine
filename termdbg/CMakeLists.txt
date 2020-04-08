

set(SOURCE_FILES
	main.cpp
	BreakpointManager.hpp
	widgets/Header.hpp
	widgets/Helpers.hpp
	widgets/RegistersView.hpp
	widgets/DisasmView.hpp
	widgets/SingleLineTextInput.hpp
	widgets/HideableElement.hpp
	widgets/CommandInput.hpp
	widgets/AddressInput.hpp
	widgets/MemoryView.hpp
	widgets/Table.hpp
	widgets/LogView.hpp
	widgets/MemorySegmentInfo.hpp
	widgets/InfoTable.hpp
	widgets/RightAlignedText.hpp
	widgets/MemorySegmentList.hpp
	widgets/MemoryBrowser.hpp
	widgets/ExecutionControlView.hpp
	widgets/BreakpointManagerView.hpp
	widgets/TopMenuView.hpp
	widgets/MainWindow.hpp
)
# CPPurses is cloned into a directory named external/
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cppurses ${CMAKE_BINARY_DIR}/external/cppurses)
add_executable(termdbg ${SOURCE_FILES})
target_include_directories(termdbg PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_include_directories(termdbg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(termdbg micromachine cppurses)
