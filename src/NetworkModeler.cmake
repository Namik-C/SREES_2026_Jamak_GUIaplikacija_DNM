set(APP_NAME NetworkModeler)			#Naziv izvrsnog fajla

file(GLOB APP_SOURCES_GUI      ${CMAKE_CURRENT_LIST_DIR}/gui/*.cpp)
file(GLOB APP_INCS_GUI         ${CMAKE_CURRENT_LIST_DIR}/gui/*.h)
file(GLOB APP_SOURCES_CATALOG  ${CMAKE_CURRENT_LIST_DIR}/CatalogModel/*.cpp)
file(GLOB APP_INCS_CATALOG     ${CMAKE_CURRENT_LIST_DIR}/CatalogModel/*.h)
file(GLOB APP_SOURCES_NETWORK  ${CMAKE_CURRENT_LIST_DIR}/network/*.cpp)
file(GLOB APP_INCS_NETWORK     ${CMAKE_CURRENT_LIST_DIR}/network/*.h)

set(APP_PLIST ${CMAKE_CURRENT_LIST_DIR}/gui/Info.plist)

file(GLOB APP_INC_TD  ${NATID_SDK_INC}/td/*.h)
file(GLOB APP_INC_GUI ${NATID_SDK_INC}/gui/*.h)

# add executable
add_executable(${APP_NAME}
    ${APP_SOURCES_GUI} ${APP_INCS_GUI}
    ${APP_SOURCES_CATALOG} ${APP_INCS_CATALOG}
    ${APP_SOURCES_NETWORK} ${APP_INCS_NETWORK}
    ${APP_INC_TD} ${APP_INC_GUI}
)

target_include_directories(${APP_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

source_group("gui"           FILES ${APP_SOURCES_GUI} ${APP_INCS_GUI})
source_group("CatalogModel"  FILES ${APP_SOURCES_CATALOG} ${APP_INCS_CATALOG})
source_group("network"       FILES ${APP_SOURCES_NETWORK} ${APP_INCS_NETWORK})
source_group("inc\\td"       FILES ${APP_INC_TD})
source_group("inc\\gui"      FILES ${APP_INC_GUI})

# Matrix biblioteka (dense/sparse matrice) - platformski zavisna ekstenzija
if (WIN32)
    set(MATRIX_LIB_DEBUG "${NATID_SDK_LIB}/MatrixD.lib")
    set(MATRIX_LIB_RELEASE "${NATID_SDK_LIB}/Matrix.lib")
elseif(APPLE)
    set(MATRIX_LIB_DEBUG "${NATID_SDK_LIB}/MatrixD.dylib")
    set(MATRIX_LIB_RELEASE "${NATID_SDK_LIB}/Matrix.dylib")
else()
    set(MATRIX_LIB_DEBUG "${NATID_SDK_LIB}/MatrixD.so")
    set(MATRIX_LIB_RELEASE "${NATID_SDK_LIB}/Matrix.so")
endif()

target_link_libraries(${APP_NAME}
    debug ${MU_LIB_DEBUG} debug ${NATGUI_LIB_DEBUG} debug ${MATRIX_LIB_DEBUG}
    optimized ${MU_LIB_RELEASE} optimized ${NATGUI_LIB_RELEASE} optimized ${MATRIX_LIB_RELEASE}
)

setTargetPropertiesForGUIApp(${APP_NAME} ${APP_PLIST})

setIDEPropertiesForGUIExecutable(${APP_NAME} ${CMAKE_CURRENT_LIST_DIR})

setPlatformDLLPath(${APP_NAME})
