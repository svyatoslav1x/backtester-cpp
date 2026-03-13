#include "start_screen_tests.h"

#include <QSignalSpy>
#include <QTest>

TEST_F(StartScreenTest, SetDatasetsPopulatesComboBox) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->datasetCombo(), nullptr);

    const QStringList datasets = {"WOW.csv", "CIRCLES.csv", "SUNFLOWER.csv"};
    screen->setDatasets(datasets);

    EXPECT_EQ(screen->datasetCombo()->count(), 3);
    EXPECT_EQ(screen->datasetCombo()->itemText(0), "WOW.csv");
    EXPECT_EQ(screen->datasetCombo()->itemText(1), "CIRCLES.csv");
    EXPECT_EQ(screen->datasetCombo()->itemText(2), "SUNFLOWER.csv");
}

TEST_F(StartScreenTest, SelectedDatasetReturnsCurrentItem) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->datasetCombo(), nullptr);

    screen->setDatasets({"AAPL.csv", "SNP.csv"});
    screen->datasetCombo()->setCurrentIndex(1);

    ASSERT_TRUE(screen->selectedDataset().has_value());
    EXPECT_EQ(*screen->selectedDataset(), "SNP.csv");
}

TEST_F(StartScreenTest, SetNewsTextUpdatesText) {
    ASSERT_NE(screen, nullptr);

    const QString text = "Breaking news: A$AP ROCKY dropped his long anticipated album DONT BE DUMB.";
    screen->setNewsText(text);

    EXPECT_EQ(screen->newsText(), text);
}

TEST_F(StartScreenTest, StartButtonEmitsSignal) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->startButton(), nullptr);

    QSignalSpy spy(screen.get(), &StartScreen::startBacktestSwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->startButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(StartScreenTest, CreateStrategyButtonEmitsSignal) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->createStrategyButton(), nullptr);

    QSignalSpy spy(screen.get(), &StartScreen::createStrategySwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->createStrategyButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(StartScreenTest, ManageStrategiesButtonEmitsSignal) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->manageStrategiesButton(), nullptr);

    QSignalSpy spy(screen.get(), &StartScreen::manageStrategiesSwitch);
    ASSERT_TRUE(spy.isValid());

    QTest::mouseClick(screen->manageStrategiesButton(), Qt::LeftButton);

    EXPECT_EQ(spy.count(), 1);
}

TEST_F(StartScreenTest, SelectedDatasetReturnsNulloptWhenNoDatasetsExist) {
    ASSERT_NE(screen, nullptr);
    ASSERT_NE(screen->datasetCombo(), nullptr);

    screen->setDatasets({});

    EXPECT_FALSE(screen->selectedDataset().has_value());
}
