#include "graph.h"
#include <QMessageBox>
#include <queue>
#include <stack>
using namespace std;

Graph::Graph(int vertices) {
    numVertices = vertices;

    // Инициализация матрицы смежности
    adjacencyMatrix.resize(numVertices, vector<int>(numVertices, 0));
    for (int i = 0; i < numVertices; i++) {
        adjacencyMatrix[i][i] = 0; // Вес петли равен 0
    }
}

// Возвращение количества вершин в графе
int Graph::getNumVertices() const {
    return numVertices;
}

//Возвращение веса ребра между вершинами
int Graph::getEdgeWeight(int v1, int v2) const {
    return adjacencyMatrix[v1][v2];
}

//Добавление нового ребра в граф.
void Graph::addEdge(int v1, int v2, int weight) {
    adjacencyMatrix[v1][v2] = weight;
    adjacencyMatrix[v2][v1] = weight;
}

//Возвращает вектор, содержащий все вершины графа
vector<int> Graph::getVertices() const {
    vector<int> vertices(numVertices);
    for (int i = 0; i < numVertices; i++) {
        vertices[i] = i;
    }
    return vertices;
}

//Добавление новой вершины в граф
void Graph::addVertex() {
    numVertices++;
    adjacencyMatrix.resize(numVertices, vector<int>(numVertices, 0));
    for (int i = 0; i < numVertices - 1; i++) {
        adjacencyMatrix[i].resize(numVertices);
    }
}

//Удаление вершины из графа
void Graph::removeVertex(int vertex) {
    if (vertex < 0 || vertex >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверный номер вершины");
        return;
    }
    numVertices--;
    // Удаляем ребра, связанные с данной вершиной
    for (int i = 0; i < numVertices; i++) {
        adjacencyMatrix[i].erase(adjacencyMatrix[i].begin() + vertex);
    }

    // Удаляем вершину из матрицы смежности
    adjacencyMatrix.erase(adjacencyMatrix.begin() + vertex);
}

//Возвращение матрицы смежности графа
vector<std::vector<int>> Graph::getAdjacencyMatrix() const {
    return adjacencyMatrix;
}

//Удаление ребра из графа
void Graph::removeEdge(int v1, int v2) {
    if (v1 < 0 || v1 >= numVertices || v2 < 0 || v2 >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверные номера вершин");
        return;
    }
    adjacencyMatrix[v1][v2] = 0;
    adjacencyMatrix[v2][v1] = 0;
}

//Изменение веса ребра
void Graph::editEdgeWeight(int v1, int v2, int weight) {
    if (v1 < 0 || v1 >= numVertices || v2 < 0 || v2 >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверные номера вершин");
        return;
    }
    adjacencyMatrix[v1][v2] = weight;
    adjacencyMatrix[v2][v1] = weight;
}

void Graph::breadthFirstSearch(int startVertex) const {
    if (startVertex < 0 || startVertex >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверный номер вершины");
        return;
    }

    vector<bool> visited(numVertices, false);
    queue<int> queue;

    visited[startVertex] = true;
    queue.push(startVertex);

    QString result;

    while (!queue.empty()) {
        int currentVertex = queue.front();
        queue.pop();
        result += QString::number(currentVertex) + " ";

        for (int i = 0; i < numVertices; i++) {
            if (adjacencyMatrix[currentVertex][i] > 0 && !visited[i]) {
                visited[i] = true;
                queue.push(i);
            }
        }
    }

    QMessageBox::information(nullptr, "Результат", result);
}

void Graph::depthFirstSearch(int startVertex) const {
    if (startVertex < 0 || startVertex >= numVertices) {
        QMessageBox::critical(nullptr, "Ошибка", "Неверный номер вершины");
        return;
    }

    vector<bool> visited(numVertices, false);
    stack<int> stack;

    visited[startVertex] = true;
    stack.push(startVertex);

    QString output;

    while (!stack.empty()) {
        int currentVertex = stack.top();
        stack.pop();
        output += QString::number(currentVertex) + " ";

        for (int i = 0; i < numVertices; i++) {
            if (adjacencyMatrix[currentVertex][i] > 0 && !visited[i]) {
                visited[i] = true;
                stack.push(i);
            }
        }
    }

    QMessageBox::information(nullptr, "Результат", output);
}

void Graph::TSP(int startVertex) const {
    // Инициализация переменных
    vector<int> path;
    vector<bool> visited(numVertices, false);
    path.push_back(startVertex);
    visited[startVertex] = true;
    int currentVertex = startVertex;
    int totalDistance = 0;

    // Проходим по всем вершинам в графе
    for (int i = 0; i < numVertices - 1; i++) {
        int nextVertex = -1;
        int minDistance = INT_MAX;

        // Ищем ближайшую не посещенную вершину
        for (int j = 0; j < numVertices; j++) {
            if (!visited[j] && adjacencyMatrix[currentVertex][j] < minDistance) {
                nextVertex = j;
                minDistance = adjacencyMatrix[currentVertex][j];
            }
        }

        // Добавляем найденную вершину в путь и обновляем переменные
        path.push_back(nextVertex);
        visited[nextVertex] = true;
        totalDistance += minDistance;
        currentVertex = nextVertex;
    }

    // Добавляем начальную вершину в конец пути и вычисляем общую длину пути
    path.push_back(startVertex);
    totalDistance += adjacencyMatrix[currentVertex][startVertex];

    // Выводим результат
    QString message = "Минимальный путь, начинающийся с вершины " + QString::number(startVertex) + ": ";
    for (int i = 0; i < path.size(); i++) {
        message += QString::number(path[i]) + " -> ";
    }
    message += QString::number(startVertex) + "\n";
    message += "Общая длина пути: " + QString::number(totalDistance);
    QMessageBox::information(nullptr, "Результат", message);
}
