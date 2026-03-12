#include <gtest/gtest.h>
#include <QApplication>
#include <QMetaType>

#include "create_strategy_screen.h"

Q_DECLARE_METATYPE(CreateStrategyInput)

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    qRegisterMetaType<CreateStrategyInput>("CreateStrategyInput");
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}