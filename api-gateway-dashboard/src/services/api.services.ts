import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Observable } from 'rxjs';
import { API } from '../models/api';

@Injectable({
  providedIn: 'root'
})
export class ApiService {

  private apiUrl = 'http://localhost:8000/api'; // URL to web api

  constructor(private http: HttpClient) { }

  // Example method to get APIs
  getApis(): Observable<API[]> {
    return this.http.get<API[]>(this.apiUrl);
  }

  // Add other methods for API operations like POST, DELETE, UPDATE
}