// Assumes the server maps these routes onto api_handle_request:
//   GET /api/students -> LIST    POST /api/students -> ADD    DELETE /api/students/<id> -> DELETE
const API_URL = "/api/students";

async function callApi(url, options) {
  const response = await fetch(url, options);
  const result = await response.json();
  if (!result.ok) {
    throw new Error(result.error);
  }
  return result;
}

async function getStudents() {
  const result = await callApi(API_URL);
  return result.data;
}

async function addStudent(student) {
  const result = await callApi(API_URL, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(student),
  });
  return result.data;
}

async function deleteStudent(id) {
  await callApi(`${API_URL}/${encodeURIComponent(id)}`, { method: "DELETE" });
}

function renderTable(students, tbody) {
  tbody.replaceChildren();
  for (const s of students) {
    const row = document.createElement("tr");
    const cells = [s.id, `${s.first_name} ${s.last_name}`, s.email, s.age, s.course, s.gpa.toFixed(2)];
    for (const value of cells) {
      const td = document.createElement("td");
      td.textContent = value;   // textContent, not innerHTML: no HTML injection
      row.appendChild(td);
    }
    const actions = document.createElement("td");
    const button = document.createElement("button");
    button.textContent = "Delete";
    button.addEventListener("click", async () => {
      await deleteStudent(s.id);
      renderTable(await getStudents(), tbody);
    });
    actions.appendChild(button);
    row.appendChild(actions);
    tbody.appendChild(row);
  }
}