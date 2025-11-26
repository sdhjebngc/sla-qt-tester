#include <QtTest/QtTest>
#include "../diagramitem.h"
#include <QGraphicsScene>

class TestDiagramItem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 简单的测试用例
    void testDiagramItemCreation();
    void testDiagramItemType();
    void testDiagramItemPosition();

private:
    QGraphicsScene *scene;
};

void TestDiagramItem::initTestCase()
{
    qDebug() << "=== 开始 DiagramItem 测试 ===";
}

void TestDiagramItem::cleanupTestCase()
{
    qDebug() << "=== 完成 DiagramItem 测试 ===";
}

void TestDiagramItem::init()
{
    scene = new QGraphicsScene();
}

void TestDiagramItem::cleanup()
{
    delete scene;
    scene = nullptr;
}

void TestDiagramItem::testDiagramItemCreation()
{
    // 测试：创建 DiagramItem
    DiagramItem *item = new DiagramItem(DiagramItem::Step, nullptr);
    QVERIFY(item != nullptr);
    
    scene->addItem(item);
    QCOMPARE(scene->items().count(), 1);
    
    qDebug() << "✓ DiagramItem 创建成功";
}

void TestDiagramItem::testDiagramItemType()
{
    // 测试：不同类型的 DiagramItem
    DiagramItem *stepItem = new DiagramItem(DiagramItem::Step, nullptr);
    QCOMPARE(stepItem->diagramType(), DiagramItem::Step);
    scene->addItem(stepItem);
    
    DiagramItem *conditionalItem = new DiagramItem(DiagramItem::Conditional, nullptr);
    QCOMPARE(conditionalItem->diagramType(), DiagramItem::Conditional);
    scene->addItem(conditionalItem);
    
    QCOMPARE(scene->items().count(), 2);
    
    qDebug() << "✓ DiagramItem 类型测试通过";
}

void TestDiagramItem::testDiagramItemPosition()
{
    // 测试：DiagramItem 位置设置
    DiagramItem *item = new DiagramItem(DiagramItem::Step, nullptr);
    scene->addItem(item);
    
    QPointF pos(100, 200);
    item->setPos(pos);
    
    QCOMPARE(item->pos(), pos);
    QCOMPARE(item->x(), 100.0);
    QCOMPARE(item->y(), 200.0);
    
    qDebug() << "✓ DiagramItem 位置测试通过";
}

QTEST_MAIN(TestDiagramItem)
#include "test_diagramitem.moc"
