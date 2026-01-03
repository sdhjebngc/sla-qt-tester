/**
 * test_item_operations.cpp
 * 
 * 集成测试：图元操作功能
 * 测试 DiagramItem 的各种操作，包括颜色、大小、旋转和选择
 * 
 * 测试要点：
 * 1. 测试不同类型图元的创建和属性
 * 2. 测试图元颜色设置
 * 3. 测试图元大小调整
 * 4. 测试图元旋转
 * 5. 测试多选和批量操作
 */

#include <QtTest/QtTest>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include "../diagramitem.h"
#include "../diagramscene.h"
#include "../diagramitemgroup.h"

class TestItemOperations : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // 集成测试用例
    void testAllDiagramTypes();
    void testItemColorChange();
    void testItemSizeChange();
    void testItemRotation();
    void testItemSelection();
    void testMultipleItemsInteraction();
    void testItemGrouping();

private:
    DiagramScene *scene;
    QGraphicsView *view;
    QMenu *itemMenu;
};

void TestItemOperations::initTestCase()
{
    qDebug() << "=== 开始图元操作集成测试 ===";
}

void TestItemOperations::cleanupTestCase()
{
    qDebug() << "=== 完成图元操作集成测试 ===";
}

void TestItemOperations::init()
{
    itemMenu = new QMenu();
    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 1000, 800));
    
    view = new QGraphicsView(scene);
    view->resize(800, 600);
    view->show();
    QVERIFY(QTest::qWaitForWindowExposed(view));
}

void TestItemOperations::cleanup()
{
    delete view;
    delete scene;
    delete itemMenu;
    view = nullptr;
    scene = nullptr;
    itemMenu = nullptr;
}

void TestItemOperations::testAllDiagramTypes()
{
    qDebug() << "测试：创建所有图元类型";
    
    // 测试所有图元类型
    QList<DiagramItem::DiagramType> types = {
        DiagramItem::Step,
        DiagramItem::Conditional,
        DiagramItem::StartEnd,
        DiagramItem::Io,
        DiagramItem::circular,
        DiagramItem::Document,
        DiagramItem::PredefinedProcess,
        DiagramItem::StoredData,
        DiagramItem::Memory,
        DiagramItem::SequentialAccessStorage,
        DiagramItem::DirectAccessStorage,
        DiagramItem::Disk,
        DiagramItem::Card,
        DiagramItem::ManualInput,
        DiagramItem::PerforatedTape,
        DiagramItem::Display,
        DiagramItem::Preparation,
        DiagramItem::ManualOperation,
        DiagramItem::ParallelMode,
        DiagramItem::Hexagon
    };
    
    int x = 50;
    int y = 50;
    int col = 0;
    
    for (DiagramItem::DiagramType type : types) {
        DiagramItem *item = new DiagramItem(type, nullptr);
        QVERIFY(item != nullptr);
        QCOMPARE(item->diagramType(), type);
        
        scene->addItem(item);
        item->setPos(x, y);
        
        // 网格布局
        col++;
        if (col >= 5) {
            col = 0;
            x = 50;
            y += 120;
        } else {
            x += 150;
        }
    }
    
    QCOMPARE(scene->items().count(), types.count());
    qDebug() << "✓ 成功创建" << types.count() << "种图元类型";
}

void TestItemOperations::testItemColorChange()
{
    qDebug() << "测试：图元颜色更改";
    
    DiagramItem *item = new DiagramItem(DiagramItem::Step, nullptr);
    scene->addItem(item);
    item->setPos(100, 100);
    
    // 测试设置不同颜色
    QColor colors[] = {Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::cyan};
    
    for (const QColor &color : colors) {
        QColor c = color;  // 创建非 const 副本
        item->setBrush(c);
        QCOMPARE(item->m_color, color);
    }
    
    qDebug() << "✓ 图元颜色更改测试通过";
}

void TestItemOperations::testItemSizeChange()
{
    qDebug() << "测试：图元大小调整";
    
    DiagramItem *item = new DiagramItem(DiagramItem::Conditional, nullptr);
    scene->addItem(item);
    item->setPos(200, 200);
    
    // 获取初始大小
    QSizeF initialSize = item->getSize();
    QVERIFY(initialSize.width() > 0);
    QVERIFY(initialSize.height() > 0);
    
    // 设置新大小
    QSizeF newSize(150, 100);
    item->setSize(newSize);
    
    // 验证大小已更新
    QSizeF currentSize = item->getSize();
    QCOMPARE(currentSize.width(), 150.0);
    QCOMPARE(currentSize.height(), 100.0);
    
    // 单独设置宽度和高度
    item->setWidth(200);
    item->setHeight(150);
    
    currentSize = item->getSize();
    QCOMPARE(currentSize.width(), 200.0);
    QCOMPARE(currentSize.height(), 150.0);
    
    qDebug() << "✓ 图元大小调整测试通过";
}

void TestItemOperations::testItemRotation()
{
    qDebug() << "测试：图元旋转";
    
    DiagramItem *item = new DiagramItem(DiagramItem::Io, nullptr);
    scene->addItem(item);
    item->setPos(300, 200);
    
    // 初始旋转角度应为0
    QCOMPARE(item->rotationAngle(), 0.0);
    
    // 设置旋转角度
    qreal angles[] = {45.0, 90.0, 180.0, 270.0, 360.0};
    
    for (qreal angle : angles) {
        item->setRotationAngle(angle);
        QCOMPARE(item->rotationAngle(), angle);
    }
    
    // 测试负角度
    item->setRotationAngle(-45.0);
    QCOMPARE(item->rotationAngle(), -45.0);
    
    qDebug() << "✓ 图元旋转测试通过";
}

void TestItemOperations::testItemSelection()
{
    qDebug() << "测试：图元选择";
    
    // 创建多个图元
    DiagramItem *item1 = new DiagramItem(DiagramItem::Step, nullptr);
    DiagramItem *item2 = new DiagramItem(DiagramItem::Conditional, nullptr);
    DiagramItem *item3 = new DiagramItem(DiagramItem::StartEnd, nullptr);
    
    scene->addItem(item1);
    scene->addItem(item2);
    scene->addItem(item3);
    
    item1->setPos(100, 100);
    item2->setPos(250, 100);
    item3->setPos(400, 100);
    
    // 启用选择
    item1->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item2->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item3->setFlag(QGraphicsItem::ItemIsSelectable, true);
    
    // 选择第一个图元
    item1->setSelected(true);
    QVERIFY(item1->isSelected());
    QVERIFY(!item2->isSelected());
    QVERIFY(!item3->isSelected());
    
    // 验证选中项目数量
    QList<QGraphicsItem *> selected = scene->selectedItems();
    QCOMPARE(selected.count(), 1);
    
    // 多选
    item2->setSelected(true);
    item3->setSelected(true);
    
    selected = scene->selectedItems();
    QCOMPARE(selected.count(), 3);
    
    // 清除选择
    scene->clearSelection();
    selected = scene->selectedItems();
    QCOMPARE(selected.count(), 0);
    
    qDebug() << "✓ 图元选择测试通过";
}

void TestItemOperations::testMultipleItemsInteraction()
{
    qDebug() << "测试：多图元交互";
    
    // 创建多个图元并定位
    QList<DiagramItem *> items;
    for (int i = 0; i < 5; i++) {
        DiagramItem *item = new DiagramItem(DiagramItem::Step, nullptr);
        scene->addItem(item);
        item->setPos(100 + i * 120, 200);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        items.append(item);
    }
    
    QCOMPARE(scene->items().count(), 5);
    
    // 全选
    for (DiagramItem *item : items) {
        item->setSelected(true);
    }
    QCOMPARE(scene->selectedItems().count(), 5);
    
    // 批量设置颜色
    QColor batchColor = Qt::magenta;
    for (DiagramItem *item : items) {
        QColor c = batchColor;
        item->setBrush(c);
    }
    
    // 验证所有图元颜色
    for (DiagramItem *item : items) {
        QCOMPARE(item->m_color, batchColor);
    }
    
    // 批量移动（模拟）
    for (int i = 0; i < items.count(); i++) {
        QPointF oldPos = items[i]->pos();
        items[i]->setPos(oldPos.x(), oldPos.y() + 50);
    }
    
    // 验证所有图元Y坐标增加了50
    for (DiagramItem *item : items) {
        QCOMPARE(item->pos().y(), 250.0);
    }
    
    qDebug() << "✓ 多图元交互测试通过";
}

void TestItemOperations::testItemGrouping()
{
    qDebug() << "测试：图元组合功能";
    
    // 创建多个图元
    DiagramItem *item1 = new DiagramItem(DiagramItem::Step, nullptr);
    DiagramItem *item2 = new DiagramItem(DiagramItem::Conditional, nullptr);
    DiagramItem *item3 = new DiagramItem(DiagramItem::Io, nullptr);
    
    scene->addItem(item1);
    scene->addItem(item2);
    scene->addItem(item3);
    
    item1->setPos(100, 100);
    item2->setPos(200, 100);
    item3->setPos(150, 200);
    
    // 创建组合
    DiagramItemGroup *group = new DiagramItemGroup();
    scene->addItem(group);
    
    group->addItem(item1);
    group->addItem(item2);
    group->addItem(item3);
    
    // 验证组合包含所有图元
    QList<QGraphicsItem *> groupChildren = group->childItems();
    QCOMPARE(groupChildren.count(), 3);
    
    // 获取组合的边界
    QRectF bounds = group->boundingRect();
    QVERIFY(bounds.width() > 0);
    QVERIFY(bounds.height() > 0);
    
    // 获取组合左上角
    QPointF topLeft = group->getTopLeft();
    QVERIFY(topLeft.x() <= 100);  // 最左边的图元在 x=100
    QVERIFY(topLeft.y() <= 100);  // 最上边的图元在 y=100
    
    qDebug() << "✓ 图元组合功能测试通过";
}

QTEST_MAIN(TestItemOperations)
#include "test_item_operations.moc"

