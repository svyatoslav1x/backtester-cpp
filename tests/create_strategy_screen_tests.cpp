#include <gtest/gtest.h>

#include "create_strategy_screen_tests.h"

#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(CreateStrategyInput)

TEST_F(CreateStrategyScreenTest, InputReturnsCurrentValues) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->nameEdit(), nullptr);
    ASSERT_NE(screen->typeCombo(), nullptr);
    ASSERT_NE(screen->shortWindowSpin(), nullptr);
    ASSERT_NE(screen->longWindowSpin(), nullptr);
    ASSERT_NE(screen->longWindowSpin(), nullptr);

    screen->nameEdit()->setText("AHA I CAN WRITE ANYTHING IN TESTS");
    screen->typeCombo()->setCurrentText("MovingAveragesLongStrategy");
    screen->shortWindowSpin()->setValue(15);
    screen->longWindowSpin()->setValue(60);

    const auto input = screen->input();

    EXPECT_EQ(input.name, "AHA I CAN WRITE ANYTHING IN TESTS");
    EXPECT_EQ(input.type, "MovingAveragesLongStrategy");
    ASSERT_TRUE(std::holds_alternative<MovingAverageParams>(input.params));
    const auto &params = std::get<MovingAverageParams>(input.params);
    EXPECT_EQ(params.shortWindow, 15);
    EXPECT_EQ(params.longWindow, 60);
}

TEST_F(CreateStrategyScreenTest, ResetFormRestoresDefaults) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->nameEdit(), nullptr);
    ASSERT_NE(screen->typeCombo(), nullptr);
    ASSERT_NE(screen->shortWindowSpin(), nullptr);
    ASSERT_NE(screen->longWindowSpin(), nullptr);
    ASSERT_NE(screen->stopLossSpin(), nullptr);

    screen->nameEdit()->setText("Changed");
    screen->typeCombo()->setCurrentText("StopLossStrategy");
    screen->shortWindowSpin()->setValue(20);
    screen->longWindowSpin()->setValue(80);
    screen->stopLossSpin()->setValue(0.90);

    screen->resetForm();

    const auto input = screen->input();

    EXPECT_TRUE(input.name.isEmpty());
    EXPECT_EQ(input.type, "MovingAveragesLongStrategy");
    ASSERT_TRUE(std::holds_alternative<MovingAverageParams>(input.params));
    const auto &params = std::get<MovingAverageParams>(input.params);
    EXPECT_EQ(params.shortWindow, 12);
    EXPECT_EQ(params.longWindow, 50);
}

TEST_F(CreateStrategyScreenTest, ParameterVisibilityMatchesSelectedType) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->typeCombo(), nullptr);

    screen->typeCombo()->setCurrentText("MovingAveragesLongStrategy");
    screen->updateParameterVisibility();

    EXPECT_FALSE(screen->shortWindowSpin()->isHidden());
    EXPECT_FALSE(screen->longWindowSpin()->isHidden());
    EXPECT_TRUE(screen->stopLossSpin()->isHidden());

    screen->typeCombo()->setCurrentText("StopLossStrategy");
    screen->updateParameterVisibility();

    EXPECT_TRUE(screen->shortWindowSpin()->isHidden());
    EXPECT_TRUE(screen->longWindowSpin()->isHidden());
    EXPECT_FALSE(screen->stopLossSpin()->isHidden());
}

TEST_F(CreateStrategyScreenTest, WindowConstraintsStayConsistent) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->shortWindowSpin(), nullptr);
    ASSERT_NE(screen->longWindowSpin(), nullptr);

    screen->shortWindowSpin()->setValue(30);
    EXPECT_GT(screen->longWindowSpin()->minimum(), 30);
    EXPECT_LT(screen->shortWindowSpin()->value(), screen->longWindowSpin()->value());

    screen->longWindowSpin()->setValue(35);
    EXPECT_LT(screen->shortWindowSpin()->maximum(), 35);
    EXPECT_LT(screen->shortWindowSpin()->value(), screen->longWindowSpin()->value());
}

TEST_F(CreateStrategyScreenTest, SaveButtonEmitsCurrentInput) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->saveButton(), nullptr);
    ASSERT_NE(screen->typeCombo(), nullptr);
    ASSERT_NE(screen->shortWindowSpin(), nullptr);
    ASSERT_NE(screen->nameEdit(), nullptr);
    ASSERT_NE(screen->longWindowSpin(), nullptr);

    screen->nameEdit()->setText("HOW CAN THEY MAKE FOLLOW THE RULES");
    screen->typeCombo()->setCurrentText("MovingAveragesLongStrategy");
    screen->shortWindowSpin()->setValue(10);
    screen->longWindowSpin()->setValue(40);

    QSignalSpy spy(screen.get(), &CreateStrategyScreen::saveStrategyRequested);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->saveButton(), Qt::LeftButton);

    ASSERT_EQ(spy.count(), 1);

    const auto arguments = spy.takeFirst();
    ASSERT_EQ(arguments.size(), 1);

    const auto input = qvariant_cast<CreateStrategyInput>(arguments[0]);
    EXPECT_EQ(input.name, "HOW CAN THEY MAKE FOLLOW THE RULES");
    EXPECT_EQ(input.type, "MovingAveragesLongStrategy");
    ASSERT_TRUE(std::holds_alternative<MovingAverageParams>(input.params));
    const auto &params = std::get<MovingAverageParams>(input.params);
    EXPECT_EQ(params.shortWindow, 10);
    EXPECT_EQ(params.longWindow, 40);
}

TEST_F(CreateStrategyScreenTest, BackButtonEmitsStartScreenSwitch) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->backButton(), nullptr);

    QSignalSpy spy(screen.get(), &CreateStrategyScreen::StartScreenSwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->backButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(CreateStrategyScreenTest, InputReturnsStopLossVariantForStopLossStrategy) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->nameEdit(), nullptr);
    ASSERT_NE(screen->typeCombo(), nullptr);
    ASSERT_NE(screen->stopLossSpin(), nullptr);

    screen->nameEdit()->setText("Stop Loss Custom");
    screen->typeCombo()->setCurrentText("StopLossStrategy");
    screen->stopLossSpin()->setValue(0.91);

    const auto input = screen->input();

    EXPECT_EQ(input.type, "StopLossStrategy");
    ASSERT_TRUE(std::holds_alternative<StopLossParams>(input.params));
    const auto &params = std::get<StopLossParams>(input.params);
    EXPECT_DOUBLE_EQ(params.stopLossPercentage, 0.91);
}
