from typing import Annotated

from fastapi import Depends, FastAPI, HTTPException, Query
from sqlmodel import Field, Session, SQLModel, create_engine, select


# Database setup
class AccessCode(SQLModel, table=True):
    id: int | None = Field(default=None, primary_key=True)
    owner: str = Field(index=True)
    password: int | None = Field(default=None, index=True)


# Create the database engine
sqlite_file_name = "database.db"
sqlite_url = f"sqlite:///{sqlite_file_name}"

connect_args = {"check_same_thread": False}
engine = create_engine(sqlite_url, connect_args=connect_args)


# Create the database and tables on startup
def create_db_and_tables():
    SQLModel.metadata.create_all(engine)

def get_session():
    with Session(engine) as session:
        yield session


SessionDep = Annotated[Session, Depends(get_session)]

app = FastAPI()

@app.on_event("startup")
def on_startup():
    create_db_and_tables()


@app.get("/")
def read_root():
    return {"Hello": "World"}


# Endpoint to create a new access code
@app.post("/create-access-codes/")
def create_access_code(
    access_code: AccessCode,
    session: SessionDep,
) -> AccessCode:
    session.add(access_code)
    session.commit()
    session.refresh(access_code)
    return access_code


# Endpoint to read all access codes 
@app.get("/read-access-codes/")
def read_access_codes(
    session: SessionDep,
    offset: int = 0,
    limit: Annotated[int, Query(le=100)] = 100,
) -> list[AccessCode]:
    access_codes = session.exec(select(AccessCode).offset(offset).limit(limit)).all()
    return access_codes # type: ignore



# Endpoint to read a specific access code by password
@app.get("/read-access-codes/{password}")
def read_access_code_by_password(password: int, session: SessionDep) -> AccessCode:
    access_code = session.exec(select(AccessCode).where(AccessCode.password == password)).first()
    if not access_code:
        raise HTTPException(status_code=404, detail="Access code not found")
    return access_code



